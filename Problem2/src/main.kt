import org.w3c.dom.HTMLElement
import org.w3c.dom.HTMLTableElement
import org.w3c.dom.events.Event
import org.w3c.dom.events.KeyboardEvent
import org.w3c.dom.get
import kotlin.browser.document

fun buildTable() {
    val body = document.getElementsByTagName("body")[0]

    val tbl = document.createElement("table") as HTMLTableElement
    val tblBody = document.createElement("tbody") as HTMLElement

    for (row_ind in 0..20) {
        val row = document.createElement("tr")

        val headerCell = document.createElement("th") as HTMLElement
        if (row_ind != 0) {
            headerCell.innerText = row_ind.toString()
        }
        headerCell.style.width = "40px"
        headerCell.id = "$row_ind"
        row.appendChild(headerCell)

        for (col_ind in 'A'..'Z') {
            val cell : HTMLElement
            if (row_ind == 0) {
                cell = document.createElement("th") as HTMLElement
                cell.innerText = col_ind.toString()
            } else {
                cell = document.createElement("td") as HTMLElement
                cell.contentEditable = "true"
                cell.addEventListener("keypress", { e : Event ->
                    e as KeyboardEvent
                    if (e.keyCode == 13) {
                        if (cell.innerText.startsWith('=')) {
                            cell.innerText = eval(cell.innerText.substring(1)).toString()
                        }
                        e.preventDefault()
                    }
                })
            }
            cell.style.width = "100px"
            cell.id = "${row_ind}_$col_ind"
            row.appendChild(cell)
        }

        tblBody.appendChild(row)
    }

    tbl.appendChild(tblBody)
    body?.appendChild(tbl)
    tbl.setAttribute("border", "1")
}

fun main(args: Array<String>) {
    buildTable()
}