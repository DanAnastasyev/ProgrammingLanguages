package ru.mipt.proglangs.sheets.parser

import kotlin.coroutines.experimental.SequenceBuilder
import kotlin.coroutines.experimental.buildIterator

interface Token

class OperatorToken(val operation : Char) : Token {
    override fun equals(other: Any?): Boolean = if (other is OperatorToken) operation == other.operation else false

    override fun hashCode() = operation.hashCode()
}

class NumberToken(text : String) : Token {
    val value = text.toDouble()
}

class FunctionToken(val text : String) : Token

class CellToken(text : String) : Token {
    val rowIndex = text.substring(1).toInt()
    val columnIndex = text[0]
}

class StringToken(val text : String) : Token

enum class SymbolType {
    DIGIT, LETTER, OPERATOR, WHITESPACE, QUOTE, UNDEFINED
}

val unaryMinus = OperatorToken('—')
val openParenthesis = OperatorToken('(')
val closeParenthesis = OperatorToken(')')

class ExpressionTokenizer(private val line : String, private val specialTokens : HashSet<String>) {
    private val operators = hashSetOf('+', '-', '*', '/', '(', ')')
    private val cellRegex = Regex("[A-Z][0-9]{1,2}")
    private val numberRegex = Regex("([0-9]+)|([0-9]*\\.[0-9]+)")

    fun getTokens(): Iterator<Token> = buildIterator {
        var token = ""
        var isUnaryMinusPossible = true
        var isInsideString = false

        for (pos in 0 until line.length) {
            if (isInsideString && line[pos] != '"') {
                token += line[pos];
                continue
            }
            when {
                line[pos] in operators -> {
                    handleLongToken(token)
                    if (line[pos] == '-' && isUnaryMinusPossible) {
                        yield(OperatorToken('—'))
                    } else {
                        yield(OperatorToken(line[pos]))
                    }
                    token = ""
                    isUnaryMinusPossible = line[pos] != ')'
                }
                line[pos].isWhitespace() -> {
                    handleLongToken(token)
                    token = ""
                }
                isLongTokenSymbol(line[pos]) -> {
                    token += line[pos]
                    isUnaryMinusPossible = false
                }
                line[pos] == '"' -> {
                    if (isInsideString) {
                        yield(StringToken(token))
                        token = ""
                    } else if (token.isNotBlank()) {
                        throw Exception("Cannot understand quotation symbol in position " + pos)
                    }
                    isInsideString = !isInsideString
                }
                else -> throw Exception("Unknown token: " + line[pos])
            }
        }
        handleLongToken(token)
    }

    private suspend fun SequenceBuilder<Token>.handleLongToken(token : String) {
        if (token.isBlank()) {
            return
        }
        yield(when {
            specialTokens.contains(token) -> FunctionToken(token)
            numberRegex.matches(token) -> NumberToken(token)
            cellRegex.matches(token) -> CellToken(token)
            else -> throw Exception("Wrong token: " + token)
        })
    }

    private fun isLongTokenSymbol(symb : Char) = symb.isDigit() || symb == '.' || symb.isLetter()
}
