# Generación de los documentos .docx

Los documentos entregables se escriben en Markdown (`docs/*.md`), que es la
fuente autoritativa, y se convierten a `.docx` con `md2docx.js`. Si hay que
corregir algo, **se corrige el `.md` y se regenera**: editar el `.docx` a mano
deja las dos versiones desincronizadas.

## Requisitos

Node.js y el paquete `docx`. No forman parte del proyecto de C++ —el simulador
sigue sin dependencias externas—; son herramientas de autoría de documentos.

```bash
npm install docx
```

## Uso

```bash
node scripts/docs/md2docx.js docs/documento_ieee_concurrencia.md docs/documento_ieee_concurrencia.docx ieee
node scripts/docs/md2docx.js docs/QA_Proyecto2.md docs/QA_Proyecto2.docx qa
```

## Los dos perfiles

| Perfil | Formato | Por qué |
|---|---|---|
| `ieee` | Letter, Times New Roman 10 pt, título a 1 columna, cuerpo a 2 columnas, títulos en negro | La plantilla institucional pide formato de dos columnas estilo IEEE conference para la versión final. Los títulos van en negro porque un artículo IEEE se imprime en blanco y negro |
| `qa` | Letter, Calibri, una sola columna | Es un informe interno dominado por tablas de trazabilidad anchas, que a dos columnas quedarían ilegibles |

## Regla de ancho al escribir el `.md` (perfil `ieee`)

Una columna mide 3,4 pulgadas: a Consolas 7,5 pt caben unos **59 caracteres**.

> **Los bloques de código no deben pasar de 50 caracteres por línea.**

Si se pasan, el texto se desborda sobre la columna vecina. El generador lo
detecta y avisa antes de escribir el `.docx`:

```
AVISO: 1 linea(s) de codigo superan 50 caracteres
       y se desbordaran de la columna. Acortelas en el .md:
       52 car.:     // --- SECCION CRITICA (RNF-2): SIN PROTEGER ---
```

Una versión anterior resolvía esto mandando cada bloque de código ancho a su
propia sección de página completa. Funcionaba, pero rompía el flujo a dos
columnas en cada fragmento de código y dejaba media página en blanco antes de
cada uno. Es preferible escribir el código estrecho.

## Regla de ancho para las tablas

**Una tabla de más de 4 columnas no cabe en una columna de texto.** El
mecanismo de escape existe —5 o más columnas, o filas de más de 180 caracteres,
abren su propia sección a página completa— pero **conviene no llegar a usarlo**:
cada sección de página completa obliga a cerrar y reabrir el flujo a dos
columnas, y Word equilibra las columnas antes del corte, dejando huecos.

En la versión actual del documento **ninguna tabla lo activa**: el cuerpo entero
es una sola sección continua a dos columnas. Se consiguió de dos formas:

- **Transponiendo** las tablas de resultados. `Versión × 8 métricas` pasó a
  `Métrica × 3 versiones` (A, B, C): mismos datos, 4 columnas en vez de 8, y
  además se comparan mejor las versiones entre sí, que es de lo que trata la
  tabla.
- **Convirtiendo a prosa** la autoevaluación ISO/IEC 25010. Eran 3 columnas,
  pero cada fila llevaba un párrafo entero de justificación; como tabla a media
  página quedaba en una tira vertical ilegible. Ahora es un párrafo por
  característica con el encabezado en negrita, más una tabla pequeña de
  resumen. El contenido es idéntico.

Los anchos de columna se reparten **en proporción al contenido**, no en partes
iguales: si no, «Corridas c/anomalía» tendría el mismo ancho que «0 / 5».

Cada fila lleva `cantSplit`, para que no se parta entre dos páginas dejando
arriba las celdas cortas vacías y abajo la continuación del texto largo.

## Conversión a PDF

No hay LibreOffice en el entorno de desarrollo; los PDF se generaron con Word:

```powershell
$w = New-Object -ComObject Word.Application
$w.Visible = $false
$d = $w.Documents.Open("<ruta>\documento.docx", [ref]$false, [ref]$false)
$d.ExportAsFixedFormat("<ruta>\documento.pdf", 17)
$d.Close([ref]$false); $w.Quit()
```
