import org.w3c.dom.HTMLElement
import org.w3c.dom.HTMLTableElement
import org.w3c.dom.events.Event
import org.w3c.dom.events.KeyboardEvent
import org.w3c.dom.get
import kotlin.browser.document


class Sheet : Table {
    //val rawTable = Array<Array<String>>(20, { Array('Z' - 'A' + 1, {""}) })
    private val parser = ExpressionParser(this)

    override fun getCellText(rowInd : Int, colInd : Char) : String {
        val cell = document.getElementById("${rowInd}_$colInd") as HTMLElement
        return cell.innerText
    }

    fun buildTable() {
        val body = document.getElementsByTagName("body")[0]

        val tbl = document.createElement("table") as HTMLTableElement
        val tblBody = document.createElement("tbody") as HTMLElement

        for (rowInd in 0..20) {
            val row = document.createElement("tr")

            val headerCell = document.createElement("th") as HTMLElement
            if (rowInd != 0) {
                headerCell.innerText = rowInd.toString()
            }
            headerCell.style.width = "40px"
            headerCell.id = "$rowInd"
            row.appendChild(headerCell)

            ('A'..'Z').map { buildCell(rowInd, it) }
                      .forEach { row.appendChild(it) }

            tblBody.appendChild(row)
        }

        tbl.appendChild(tblBody)
        body?.appendChild(tbl)
        tbl.setAttribute("border", "1")
    }

    private fun buildCell(rowInd : Int, colInd : Char): HTMLElement {
        val cell : HTMLElement
        if (rowInd == 0) {
            cell = document.createElement("th") as HTMLElement
            cell.innerText = colInd.toString()
        } else {
            cell = document.createElement("td") as HTMLElement
            cell.contentEditable = "true"
            cell.addEventListener("keypress", { e: Event ->
                e as KeyboardEvent
                if (e.keyCode == 13) {
                    if (cell.innerText.startsWith('=')) {
                        try {
                            cell.innerText = parser.parse(cell.innerText.substring(1)).toString()
                        } catch (ex : Throwable) {
                            cell.innerText = ex.toString()
                        }
                    }
                    e.preventDefault()
                }
            })
        }
        cell.style.width = "100px"
        cell.id = "${rowInd}_$colInd"
        return cell
    }
}

fun main(args: Array<String>) {
    val s = Sheet()
    s.buildTable()
}