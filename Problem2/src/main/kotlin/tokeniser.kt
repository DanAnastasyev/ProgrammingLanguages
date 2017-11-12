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
            val tokenType = when {
                line[pos].isDigit() || line[pos] == '.' -> SymbolType.DIGIT
                line[pos] in operators -> SymbolType.OPERATOR
                line[pos].isLetter() -> SymbolType.LETTER
                line[pos].isWhitespace() -> SymbolType.WHITESPACE
                line[pos] == '"' -> SymbolType.QUOTE
                else -> SymbolType.UNDEFINED
            }
            when (tokenType) {
                SymbolType.OPERATOR -> {
                    handleLongToken(token)
                    if (line[pos] == '-' && isUnaryMinusPossible) {
                        yield(OperatorToken('—'))
                    } else {
                        yield(OperatorToken(line[pos]))
                    }
                    token = ""
                    isUnaryMinusPossible = line[pos] != ')'
                }
                SymbolType.WHITESPACE -> {
                    handleLongToken(token)
                    token = ""
                }
                SymbolType.LETTER, SymbolType.DIGIT -> {
                    token += line[pos]
                    isUnaryMinusPossible = false
                }
                SymbolType.QUOTE -> {
                    if (isInsideString) {
                        yield(StringToken(token))
                        token = ""
                    } else if (token.isNotBlank()) {
                        throw Exception("Cannot understand quotation symbol in position " + pos)
                    }
                    isInsideString = !isInsideString
                }
                SymbolType.UNDEFINED -> throw Exception("Unknown token: " + line[pos])
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
}
