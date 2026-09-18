# exportar_pdf.py -- DOCX -> PDF con Word, rellenando antes el indice.
#
# El generador escribe el indice como campo TOC vacio: solo Word sabe en que
# pagina cae cada titulo. Aqui se abre el documento, se actualizan todos los
# campos (indice y "Pagina X de Y"), se GUARDA el .docx ya relleno -para que
# quien lo abra en Word vea el indice sin pulsar nada- y se exporta el PDF con
# marcadores de navegacion por titulo.
#
# Uso: python scripts/exportar_pdf.py <entrada.docx> <salida.pdf>

import os
import sys
import win32com.client

WD_EXPORT_PDF = 17
WD_EXPORT_OPTIMIZE_PRINT = 0
WD_EXPORT_BOOKMARKS_HEADINGS = 1


def exportar(docx, pdf):
    docx, pdf = os.path.abspath(docx), os.path.abspath(pdf)
    word = win32com.client.DispatchEx("Word.Application")
    word.Visible = False
    word.DisplayAlerts = 0
    try:
        doc = word.Documents.Open(docx, ConfirmConversions=False, ReadOnly=False,
                                  AddToRecentFiles=False, Visible=False)
        for toc in doc.TablesOfContents:
            toc.Update()
        doc.Fields.Update()
        doc.Repaginate()
        # Segunda pasada: rellenar el indice puede mover titulos de pagina.
        for toc in doc.TablesOfContents:
            toc.UpdatePageNumbers()
        paginas = doc.ComputeStatistics(2)  # wdStatisticPages
        doc.Save()
        doc.ExportAsFixedFormat(OutputFileName=pdf, ExportFormat=WD_EXPORT_PDF,
                                OpenAfterExport=False, OptimizeFor=WD_EXPORT_OPTIMIZE_PRINT,
                                IncludeDocProps=True, CreateBookmarks=WD_EXPORT_BOOKMARKS_HEADINGS)
        doc.Close(False)
        print(f"OK {pdf}  paginas={paginas}  bytes={os.path.getsize(pdf)}")
    finally:
        word.Quit()


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python exportar_pdf.py <entrada.docx> <salida.pdf>")
        sys.exit(2)
    exportar(sys.argv[1], sys.argv[2])
