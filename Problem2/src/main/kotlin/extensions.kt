package ru.mipt.proglangs.sheets.parser

fun <T> ArrayList<T>.pop() : T = this.removeAt(this.size - 1)

fun <T> ArrayList<T>.safePop() = if (this.isNotEmpty()) {
    this.pop()
} else {
    null
}

fun Char.isDigit() : Boolean = this in '0'..'9'

fun Char.isLetter() : Boolean = this in 'A'..'Z'
