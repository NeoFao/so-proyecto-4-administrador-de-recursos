// md2docx.js -- Conversor Markdown -> DOCX para los documentos del Proyecto 2.
//
// Replica el formato del Proyecto 1:
//   IEEE : Letter, Times New Roman, titulo a 1 columna, cuerpo a 2 columnas,
//          tablas anchas y bloques de codigo largos a pagina completa.
//   QA   : Letter, Calibri, una sola columna.
//
// Uso: node md2docx.js <entrada.md> <salida.docx> <ieee|qa>

const fs = require("fs");
const {
  Document, Packer, Paragraph, TextRun, HeadingLevel, AlignmentType,
  Table, TableRow, TableCell, WidthType, BorderStyle, ShadingType,
  SectionType, PageOrientation, LevelFormat, convertInchesToTwip,
} = require("docx");

// ---------------------------------------------------------------- parametros

const PERFILES = {
  ieee: {
    fuente: "Times New Roman",
    tamCuerpo: 20,        // half-points => 10 pt
    margen: convertInchesToTwip(0.75),
    dosColumnas: true,
    espacioColumnas: 320,
    minColumnasTablaAncha: 5,
    maxCaracteresFilaEnColumna: 180,
    // Un articulo IEEE se imprime en blanco y negro: los titulos van en negro,
    // no en el azul que Word aplica por defecto a los estilos de encabezado.
    colorTitulos: "000000",
  },
  qa: {
    fuente: "Calibri",
    tamCuerpo: 20,
    margen: convertInchesToTwip(1),
    dosColumnas: false,
    espacioColumnas: 720,
    minColumnasTablaAncha: 999,      // una sola columna: nunca hace falta
    maxCaracteresFilaEnColumna: 1e9, // idem
    colorTitulos: undefined,         // informe interno: se conserva el azul de Word
  },
};

const ANCHO_PAGINA = 12240;   // Letter
const ALTO_PAGINA = 15840;

// ------------------------------------------------------------------ parseo md

function parsearMarkdown(texto) {
  const lineas = texto.split(/\r?\n/);
  const bloques = [];
  let i = 0;
  // El bloque de encabezado (titulo, autor, afiliacion, fecha) esta escrito con
  // un dato por linea. Markdown fusionaria esas lineas en un solo parrafo, que
  // es correcto para prosa pero ilegible para una portada; hasta el primer '##'
  // cada linea se conserva como parrafo propio.
  let enEncabezado = true;
  let vistoTitulo = false;
  let contadorListas = 0;

  const esFilaTabla = (l) => /^\s*\|.*\|\s*$/.test(l);
  const esSeparadorTabla = (l) => /^\s*\|[\s:|-]+\|\s*$/.test(l);

  // Una linea en blanco entre dos items NO termina la lista: en Markdown eso es
  // una "lista suelta", el mismo bloque con mas aire. Sin esta mirada hacia
  // adelante cada item quedaba en una lista propia y, como cada lista recibe su
  // instancia de numeracion, Word reiniciaba el contador: la seccion 10 del QA
  // salia numerada "1." seis veces en vez de 1 a 6.
  const saltarHuecoDeLista = (lineas, i, patron) => {
    let j = i;
    while (j < lineas.length && lineas[j].trim() === "") j++;
    return (j > i && j < lineas.length && patron.test(lineas[j])) ? j : i;
  };

  while (i < lineas.length) {
    const linea = lineas[i];

    if (linea.trim() === "") { i++; continue; }

    // La regla horizontal cierra la portada. Sin esto, un documento cuyas partes
    // usan '#' en vez de '##' -como la guia de defensa- dejaba 'enEncabezado'
    // activo pagina tras pagina, y toda la prosa, las tablas y los bloques de
    // codigo salian centrados como si fueran la caratula.
    if (/^---+\s*$/.test(linea.trim())) {
      enEncabezado = false;
      bloques.push({ tipo: "hr" });
      i++;
      continue;
    }

    const enc = linea.match(/^(#{1,4})\s+(.*)$/);
    if (enc) {
      // Segundo cierre: cualquier encabezado que no sea el titulo del documento.
      if (enc[1].length >= 2 || vistoTitulo) enEncabezado = false;
      vistoTitulo = true;
      bloques.push({ tipo: "h" + enc[1].length, texto: enc[2].trim() });
      i++;
      continue;
    }

    if (enEncabezado) {
      bloques.push({ tipo: "meta", texto: linea.trim() });
      i++;
      continue;
    }

    if (/^```/.test(linea)) {
      const cuerpo = [];
      i++;
      while (i < lineas.length && !/^```/.test(lineas[i])) { cuerpo.push(lineas[i]); i++; }
      i++; // cierre
      bloques.push({ tipo: "code", lineas: cuerpo });
      continue;
    }

    if (esFilaTabla(linea) && i + 1 < lineas.length && esSeparadorTabla(lineas[i + 1])) {
      const partir = (l) => l.trim().replace(/^\|/, "").replace(/\|$/, "").split("|").map((c) => c.trim());
      const encabezados = partir(linea);
      i += 2;
      const filas = [];
      while (i < lineas.length && esFilaTabla(lineas[i])) { filas.push(partir(lineas[i])); i++; }
      bloques.push({ tipo: "table", encabezados, filas });
      continue;
    }

    if (/^\s*[-*]\s+/.test(linea)) {
      const items = [];
      while (i < lineas.length && /^\s*[-*]\s+/.test(lineas[i])) {
        let item = lineas[i].replace(/^\s*[-*]\s+/, "");
        i++;
        // continuacion indentada del mismo item
        while (i < lineas.length && /^\s{2,}\S/.test(lineas[i]) && !/^\s*[-*]\s+/.test(lineas[i])) {
          item += " " + lineas[i].trim();
          i++;
        }
        items.push(item);
        i = saltarHuecoDeLista(lineas, i, /^\s*[-*]\s+/);
      }
      bloques.push({ tipo: "ul", items });
      continue;
    }

    // Bloque de cita (`> texto`). Sin esta rama los ">" se imprimian
    // literalmente y todos los parrafos de la cita se fundian en uno solo,
    // que es justo lo que le pasaba a la guia de defensa: las respuestas que
    // hay que decir en voz alta quedaban en un ladrillo ilegible.
    //
    // Una linea que solo contiene ">" separa parrafos dentro de la cita.
    if (/^\s*>/.test(linea)) {
      const parrafos = [];
      let actual = "";
      while (i < lineas.length && /^\s*>/.test(lineas[i])) {
        const contenido = lineas[i].replace(/^\s*>\s?/, "");
        if (contenido.trim() === "") {
          if (actual.trim() !== "") parrafos.push(actual.trim());
          actual = "";
        } else {
          actual += (actual ? " " : "") + contenido.trim();
        }
        i++;
      }
      if (actual.trim() !== "") parrafos.push(actual.trim());
      if (parrafos.length > 0) bloques.push({ tipo: "cita", parrafos });
      continue;
    }

    if (/^\s*\d+\.\s+/.test(linea)) {
      const items = [];
      while (i < lineas.length && /^\s*\d+\.\s+/.test(lineas[i])) {
        let item = lineas[i].replace(/^\s*\d+\.\s+/, "");
        i++;
        while (i < lineas.length && /^\s{3,}\S/.test(lineas[i]) && !/^\s*\d+\.\s+/.test(lineas[i])) {
          item += " " + lineas[i].trim();
          i++;
        }
        items.push(item);
        i = saltarHuecoDeLista(lineas, i, /^\s*\d+\.\s+/);
      }
      // Cada lista numerada recibe su propia instancia. Sin esto todas
      // comparten el mismo contador y la segunda lista del documento empieza
      // en 6 en vez de en 1, porque continua donde termino la primera.
      bloques.push({ tipo: "ol", items, instancia: ++contadorListas });
      continue;
    }

    // Parrafo: acumula hasta linea en blanco o inicio de otro bloque.
    //
    // GUARDA CONTRA BUCLE INFINITO: si ninguna rama anterior consumio la linea
    // y la condicion de abajo tampoco la acepta -- el caso real es una fila de
    // tabla huerfana, un "| ... |" cuya linea siguiente no es el separador
    // "|---|" --, el bucle no incrementaria `i` y el programa se colgaria en
    // silencio. Ocurrio: un escape mal puesto partio una tabla en dos y la
    // generacion dejo de terminar, sin ningun mensaje. Se consume la linea como
    // parrafo y se avisa, para que el sintoma sea un aviso y no un cuelgue.
    const inicioParrafo = i;
    const parrafo = [];
    while (
      i < lineas.length && lineas[i].trim() !== "" &&
      !/^(#{1,4})\s/.test(lineas[i]) && !/^```/.test(lineas[i]) &&
      !esFilaTabla(lineas[i]) && !/^\s*[-*]\s+/.test(lineas[i]) &&
      !/^\s*\d+\.\s+/.test(lineas[i]) && !/^---+\s*$/.test(lineas[i].trim())
    ) {
      parrafo.push(lineas[i].trim());
      i++;
    }
    if (parrafo.length) {
      bloques.push({ tipo: "p", texto: parrafo.join(" ") });
    } else {
      console.warn(
        `AVISO linea ${inicioParrafo + 1}: no se reconocio el bloque; se emite como parrafo.\n` +
        `       Contenido: ${lineas[inicioParrafo].slice(0, 80)}\n` +
        "       Causa habitual: una fila de tabla suelta, sin la linea |---| de separacion.");
      bloques.push({ tipo: "p", texto: lineas[inicioParrafo].trim() });
      i = inicioParrafo + 1; // garantiza el avance
    }
  }

  return bloques;
}

// ------------------------------------------------- formato inline -> TextRun[]

function runsDeTexto(texto, cfg, opciones = {}) {
  const base = {
    font: opciones.mono ? "Consolas" : cfg.fuente,
    size: opciones.size || cfg.tamCuerpo,
    bold: !!opciones.bold,
    italics: !!opciones.italics,
    color: opciones.color,
  };

  if (opciones.mono) return [new TextRun({ ...base, text: texto })];

  const runs = [];
  // Un solo recorrido por los tres marcadores, para no reprocesar lo ya emitido.
  const re = /(\*\*([^*]+)\*\*)|(`([^`]+)`)|(\*([^*]+)\*)/g;
  let ultimo = 0;
  let m;
  while ((m = re.exec(texto)) !== null) {
    if (m.index > ultimo) {
      runs.push(new TextRun({ ...base, text: texto.slice(ultimo, m.index) }));
    }
    // Negrita y cursiva RECURREN sobre su contenido: sin esto, un fragmento
    // como **`Archivos.cpp`** emitia los acentos graves literales, porque el
    // interior de la negrita se trataba como texto plano. El codigo NO recurre:
    // dentro de un fragmento monoespaciado todo es literal por definicion.
    if (m[2] !== undefined) {
      runs.push(...runsDeTexto(m[2], cfg, { ...opciones, size: base.size, bold: true }));
    } else if (m[4] !== undefined) {
      runs.push(new TextRun({ ...base, text: m[4], font: "Consolas", size: base.size - 2 }));
    } else if (m[6] !== undefined) {
      runs.push(...runsDeTexto(m[6], cfg, { ...opciones, size: base.size, italics: true }));
    }
    ultimo = re.lastIndex;
  }
  if (ultimo < texto.length) runs.push(new TextRun({ ...base, text: texto.slice(ultimo) }));
  if (runs.length === 0) runs.push(new TextRun({ ...base, text: "" }));
  return runs;
}

// ------------------------------------------------------ bloques -> elementos

const BORDE = { style: BorderStyle.SINGLE, size: 4, color: "999999" };
const BORDES_CELDA = { top: BORDE, bottom: BORDE, left: BORDE, right: BORDE };

// Reparte el ancho entre columnas EN PROPORCION a su contenido, no en partes
// iguales. En una tabla como (Metrica | A | B | C), la division equitativa
// daria a "Corridas c/anomalia" el mismo ancho que a "0 / 5": la primera se
// partiria en cuatro lineas y las otras tres desperdiciarian espacio.
function anchosProporcionales(bloque, anchoDisponible) {
  const n = bloque.encabezados.length;

  const pesos = [];
  for (let c = 0; c < n; c++) {
    // La palabra mas larga marca el minimo real: una celda no puede ser mas
    // estrecha que su palabra mas larga sin partirla a la mitad.
    let masLarga = 0;
    const medir = (t) => {
      for (const palabra of String(t).split(/\s+/)) {
        if (palabra.length > masLarga) masLarga = palabra.length;
      }
    };
    medir(bloque.encabezados[c]);
    let total = bloque.encabezados[c].length;
    for (const f of bloque.filas) {
      const celda = f[c] !== undefined ? f[c] : "";
      medir(celda);
      if (celda.length > total) total = celda.length;
    }
    // Promedio entre el ancho total del texto y su palabra mas larga: evita
    // que una celda con una frase larga acapare toda la tabla.
    pesos.push(Math.max(4, (total + masLarga * 2) / 3));
  }

  const sumaPesos = pesos.reduce((a, b) => a + b, 0);
  const minimo = Math.floor(anchoDisponible * 0.09);
  const anchos = pesos.map((p) =>
    Math.max(minimo, Math.floor((anchoDisponible * p) / sumaPesos)));

  // El redondeo deja un sobrante (o un exceso): se ajusta sobre la columna mas
  // ancha, que es la que menos acusa la diferencia.
  const suma = anchos.reduce((a, b) => a + b, 0);
  let masAncha = 0;
  for (let c = 1; c < n; c++) if (anchos[c] > anchos[masAncha]) masAncha = c;
  anchos[masAncha] += anchoDisponible - suma;

  return anchos;
}

function construirTabla(bloque, cfg, anchoDisponible) {
  const n = bloque.encabezados.length;
  const anchos = anchosProporcionales(bloque, anchoDisponible);

  const tamTabla = Math.max(14, cfg.tamCuerpo - 4);

  const celda = (texto, opciones, ancho) =>
    new TableCell({
      width: { size: ancho, type: WidthType.DXA },
      borders: BORDES_CELDA,
      shading: opciones.encabezado
        ? { type: ShadingType.CLEAR, fill: "E8E8E8", color: "auto" }
        : undefined,
      margins: { top: 40, bottom: 40, left: 80, right: 80 },
      children: [
        new Paragraph({
          spacing: { before: 0, after: 0 },
          children: runsDeTexto(texto, cfg, { bold: opciones.encabezado, size: tamTabla }),
        }),
      ],
    });

  const filas = [
    new TableRow({
      tableHeader: true,
      children: bloque.encabezados.map((h, k) => celda(h, { encabezado: true }, anchos[k])),
    }),
  ];

  for (const f of bloque.filas) {
    const celdas = [];
    for (let k = 0; k < n; k++) celdas.push(celda(f[k] !== undefined ? f[k] : "", {}, anchos[k]));
    // cantSplit evita que una fila se parta entre dos paginas dejando arriba
    // las celdas cortas vacias y abajo la continuacion del texto largo, que se
    // lee como una fila huerfana. La fila entera pasa a la pagina siguiente.
    filas.push(new TableRow({ children: celdas, cantSplit: true }));
  }

  return new Table({ columnWidths: anchos, width: { size: anchoDisponible, type: WidthType.DXA }, rows: filas });
}

function construirElementos(bloque, cfg, anchoDisponible) {
  switch (bloque.tipo) {
    case "h1":
      return [new Paragraph({
        heading: HeadingLevel.HEADING_1,
        alignment: AlignmentType.CENTER,
        spacing: { before: 0, after: 200 },
        children: runsDeTexto(bloque.texto, cfg, { bold: true, size: 44, color: cfg.colorTitulos }),
      })];
    case "h2":
      return [new Paragraph({
        heading: HeadingLevel.HEADING_2,
        spacing: { before: 240, after: 120 },
        children: runsDeTexto(bloque.texto, cfg,
          { bold: true, size: cfg.tamCuerpo + 4, color: cfg.colorTitulos }),
      })];
    case "h3":
      return [new Paragraph({
        heading: HeadingLevel.HEADING_3,
        spacing: { before: 180, after: 100 },
        children: runsDeTexto(bloque.texto, cfg,
          { bold: true, size: cfg.tamCuerpo + 2, color: cfg.colorTitulos }),
      })];
    case "h4":
      return [new Paragraph({
        heading: HeadingLevel.HEADING_4,
        spacing: { before: 140, after: 80 },
        children: runsDeTexto(bloque.texto, cfg,
          { bold: true, italics: true, color: cfg.colorTitulos }),
      })];
    case "meta":
      // Linea del bloque de portada: centrada y sin justificar.
      return [new Paragraph({
        alignment: AlignmentType.CENTER,
        spacing: { before: 0, after: 60 },
        children: runsDeTexto(bloque.texto, cfg),
      })];
    case "p":
      return [new Paragraph({
        alignment: AlignmentType.JUSTIFIED,
        spacing: { before: 0, after: 120, line: 260 },
        children: runsDeTexto(bloque.texto, cfg),
      })];
    case "cita":
      // Se rinde como parrafos sangrados, en cursiva y con una barra vertical
      // a la izquierda: el lector distingue de un vistazo lo que es texto para
      // decir en voz alta de lo que es explicacion.
      return bloque.parrafos.map((texto, indice) => new Paragraph({
        alignment: AlignmentType.JUSTIFIED,
        indent: { left: convertInchesToTwip(0.3) },
        spacing: {
          before: indice === 0 ? 120 : 0,
          after: indice === bloque.parrafos.length - 1 ? 160 : 80,
          line: 260,
        },
        border: {
          left: { style: BorderStyle.SINGLE, size: 12, space: 8, color: "AAAAAA" },
        },
        children: runsDeTexto(texto, cfg, { italics: true }),
      }));
    case "ul":
      return bloque.items.map((it) => new Paragraph({
        bullet: { level: 0 },
        alignment: AlignmentType.JUSTIFIED,
        spacing: { before: 0, after: 60, line: 260 },
        children: runsDeTexto(it, cfg),
      }));
    case "ol":
      return bloque.items.map((it) => new Paragraph({
        numbering: { reference: "lista-numerada", level: 0, instance: bloque.instancia },
        alignment: AlignmentType.JUSTIFIED,
        spacing: { before: 0, after: 60, line: 260 },
        children: runsDeTexto(it, cfg),
      }));
    case "code":
      // Cada linea es su propio Paragraph: docx no admite '\n' dentro de un run.
      // keepLines/keepNext mantienen el bloque entero junto, para que no se
      // parta entre el final de una columna y el principio de la siguiente.
      return bloque.lineas.map((l, idx) => new Paragraph({
        spacing: { before: idx === 0 ? 60 : 0, after: idx === bloque.lineas.length - 1 ? 120 : 0 },
        shading: { type: ShadingType.CLEAR, fill: "F2F2F2", color: "auto" },
        indent: { left: 120 },
        keepLines: true,
        keepNext: idx < bloque.lineas.length - 1,
        children: [new TextRun({ text: l === "" ? " " : l, font: "Consolas", size: 15 })],
      }));
    case "table":
      return [construirTabla(bloque, cfg, anchoDisponible),
              new Paragraph({ spacing: { after: 120 }, children: [new TextRun({ text: "" })] })];
    case "hr":
      return [new Paragraph({
        spacing: { before: 60, after: 120 },
        border: { bottom: { style: BorderStyle.SINGLE, size: 6, color: "BBBBBB" } },
        children: [new TextRun({ text: "" })],
      })];
    default:
      return [];
  }
}

// ------------------------------------------------------ agrupado en secciones

function propiedadesSeccion(cfg, columnas, continua) {
  const props = {
    page: {
      size: { width: ANCHO_PAGINA, height: ALTO_PAGINA, orientation: PageOrientation.PORTRAIT },
      margin: { top: cfg.margen, right: cfg.margen, bottom: cfg.margen, left: cfg.margen },
    },
  };
  if (continua) props.type = SectionType.CONTINUOUS;
  if (columnas === 2) props.column = { space: cfg.espacioColumnas, count: 2, equalWidth: true };
  return props;
}

function anchoUtil(cfg, columnas) {
  const total = ANCHO_PAGINA - 2 * cfg.margen;
  if (columnas === 1) return total;
  return Math.floor((total - cfg.espacioColumnas) / 2);
}

function construirSecciones(bloques, cfg) {
  const anchoUna = anchoUtil(cfg, 1);
  const anchoDos = anchoUtil(cfg, 2);

  if (!cfg.dosColumnas) {
    const hijos = [];
    for (const b of bloques) hijos.push(...construirElementos(b, cfg, anchoUna));
    return [{ properties: propiedadesSeccion(cfg, 1, false), children: hijos }];
  }

  // Perfil IEEE: el encabezado (titulo + autoria, hasta el primer '##') va a una
  // columna; el cuerpo va a dos, y cada bloque "ancho" abre su propia seccion a
  // pagina completa para no quedar comprimido en media pagina.
  // SOLO las tablas anchas abren seccion propia. Los bloques de codigo NO:
  // hacerlo partia el flujo a dos columnas en cada fragmento de codigo y
  // dejaba media pagina en blanco antes de cada uno. Los bloques del
  // documento estan escritos para caber en el ancho de una columna (<= 50
  // caracteres); si alguno se pasa, se avisa al generar en vez de romper la
  // maquetacion en silencio.
  // Una tabla necesita pagina completa por dos motivos distintos: por tener
  // muchas columnas, o por tener pocas pero con celdas de texto largo. La
  // tabla ISO/IEC 25010 son 3 columnas, pero cada fila lleva un parrafo de
  // justificacion: a media pagina queda convertida en una tira vertical
  // ilegible. Contar solo columnas no basta.
  const filaMasLarga = (b) =>
    b.filas.reduce((max, f) => Math.max(max, f.join("").length), b.encabezados.join("").length);

  const esAncho = (b) =>
    b.tipo === "table" &&
    (b.encabezados.length >= cfg.minColumnasTablaAncha ||
     filaMasLarga(b) > cfg.maxCaracteresFilaEnColumna);

  const indicePrimerH2 = bloques.findIndex((b) => b.tipo === "h2");
  const corte = indicePrimerH2 === -1 ? bloques.length : indicePrimerH2;

  const secciones = [];
  const encabezado = [];
  for (const b of bloques.slice(0, corte)) encabezado.push(...construirElementos(b, cfg, anchoUna));
  if (encabezado.length) {
    secciones.push({ properties: propiedadesSeccion(cfg, 1, false), children: encabezado });
  }

  let acumulado = [];
  const cerrarDosColumnas = () => {
    if (acumulado.length) {
      secciones.push({ properties: propiedadesSeccion(cfg, 2, true), children: acumulado });
      acumulado = [];
    }
  };

  for (const b of bloques.slice(corte)) {
    if (esAncho(b)) {
      cerrarDosColumnas();
      secciones.push({
        properties: propiedadesSeccion(cfg, 1, true),
        children: construirElementos(b, cfg, anchoUna),
      });
    } else {
      acumulado.push(...construirElementos(b, cfg, anchoDos));
    }
  }
  cerrarDosColumnas();

  return secciones;
}

// ------------------------------------------------------------------- programa

function main() {
  const [entrada, salida, perfilNombre] = process.argv.slice(2);
  if (!entrada || !salida || !PERFILES[perfilNombre]) {
    console.error("Uso: node md2docx.js <entrada.md> <salida.docx> <ieee|qa>");
    process.exit(2);
  }

  const cfg = PERFILES[perfilNombre];
  const bloques = parsearMarkdown(fs.readFileSync(entrada, "utf8"));

  // Aviso de ancho: a Consolas 7,5 pt caben unos 59 caracteres en una columna
  // de 3,4 pulgadas. Se avisa a partir de 50 para dejar margen. Una linea mas
  // larga se desborda sobre la columna vecina, que es exactamente el defecto
  // que rompia la maquetacion antes de estrechar los bloques.
  if (cfg.dosColumnas) {
    const LIMITE = 50;
    const excesos = [];
    for (const b of bloques) {
      if (b.tipo !== "code") continue;
      for (const l of b.lineas) {
        if (l.length > LIMITE) excesos.push(`${l.length} car.: ${l.slice(0, 60)}`);
      }
    }
    if (excesos.length) {
      console.warn(`AVISO: ${excesos.length} linea(s) de codigo superan ${LIMITE} caracteres`);
      console.warn("       y se desbordaran de la columna. Acortelas en el .md:");
      for (const e of excesos.slice(0, 10)) console.warn("       " + e);
    }
  }

  const secciones = construirSecciones(bloques, cfg);

  const doc = new Document({
    creator: "Fabrizio Espinoza Arce",
    title: perfilNombre === "ieee"
      ? "Simulador de Concurrencia -- Documento IEEE"
      : "Documento de Aseguramiento de Calidad -- Proyecto 2",
    description: "TIIT2007 Sistemas Operativos, Universidad Invenio",
    numbering: {
      config: [{
        reference: "lista-numerada",
        levels: [{
          level: 0, format: LevelFormat.DECIMAL, text: "%1.", alignment: AlignmentType.START,
          style: { paragraph: { indent: { left: 480, hanging: 240 } } },
        }],
      }],
    },
    styles: {
      default: {
        document: { run: { font: cfg.fuente, size: cfg.tamCuerpo } },
      },
    },
    sections: secciones,
  });

  Packer.toBuffer(doc).then((buffer) => {
    fs.writeFileSync(salida, buffer);
    const tablas = bloques.filter((b) => b.tipo === "table").length;
    console.log(
      `OK ${salida}\n   bloques=${bloques.length} tablas=${tablas} secciones=${secciones.length} bytes=${buffer.length}`
    );
  }).catch((e) => { console.error("ERROR:", e); process.exit(1); });
}

main();
