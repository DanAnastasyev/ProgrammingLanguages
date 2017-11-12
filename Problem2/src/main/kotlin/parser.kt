import kotlin.coroutines.experimental.SequenceBuilder
import kotlin.coroutines.experimental.buildIterator
import kotlin.js.Math

fun <T> ArrayList<T>.pop() : T = this.removeAt(this.size - 1)

fun <T> ArrayList<T>.safePop() = if (this.isNotEmpty()) {
    this.pop()
} else {
    null
}

fun Char.isDigit() : Boolean = this in '0'..'9'

fun Char.isLetter() : Boolean = this in 'A'..'Z'

class Token(val text: String) {
    override fun toString() = text
}

enum class SymbolType {
    DIGIT, LETTER, OPERATOR, WHITESPACE, UNDEFINED
}

class ExpressionTokenizer(private val line : String, private val specialTokens : HashSet<String>) {
    private val operators = hashSetOf('+', '-', '*', '/', '(', ')')
    private val cellRegex = Regex("[A-Z][0-9]{1,2}")
    private val digitRegex = Regex("([0-9]+)|([0-9]*\\.[0-9]+)")

    private fun isGoodToken(token : String): Boolean {
        return specialTokens.contains(token) or digitRegex.matches(token) or cellRegex.matches(token);
    }

    private suspend fun SequenceBuilder<Token>.handleLongToken(token : String) {
        if (token.isNotBlank()) {
            if (isGoodToken(token)) {
                yield(Token(token))
            } else {
                throw Exception("Wrong token: " + token)
            }
        }
    }

    fun getTokens(): Iterator<Token> = buildIterator {
        var token = ""
        var isUnaryMinusPossible = true

        for (pos in 0 until line.length) {
            val tokenType = when {
                line[pos].isDigit() || line[pos] == '.' -> SymbolType.DIGIT
                line[pos] in operators -> SymbolType.OPERATOR
                line[pos].isLetter() -> SymbolType.LETTER
                line[pos].isWhitespace() -> SymbolType.WHITESPACE
                else -> SymbolType.UNDEFINED
            }
            when (tokenType) {
                SymbolType.OPERATOR -> {
                    handleLongToken(token)
                    if (line[pos] == '-' && isUnaryMinusPossible) {
                        yield(Token("—"))
                    } else {
                        yield(Token(line.substring(pos, pos + 1)))
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
                SymbolType.UNDEFINED -> throw Exception("Parsing error")
            }
        }
        handleLongToken(token)
    }
}

interface Expression {
    fun eval() : Double
}

class Value(private val value : Double) : Expression {
    override fun eval() = value
}

class Operator(private val left : Expression, private val operation : String,
               private val right : Expression) : Expression {

    override fun eval() : Double = when (operation) {
        "+" -> left.eval() + right.eval()
        "-" -> left.eval() - right.eval()
        "*" -> left.eval() * right.eval()
        "/" -> left.eval() / right.eval()
        else -> throw Exception("Wrong operation type")
    }
}

class Function(private val function : String,
               private val arg : Expression) : Expression {

    override fun eval() : Double = when (function) {
        "ABS" -> Math.abs(arg.eval())
        "SIN" -> Math.sin(arg.eval())
        "LEN" -> throw Exception("Not Implemented")
        else -> throw Exception("Wrong operation type")
    }
}

class ExpressionParser {
    private val functionTokens = hashSetOf("ABS", "SIN", "LEN")

    fun parse(expr : String) : Double {
        val expressionStack = ArrayList<Expression>()
        val operatorStack = ArrayList<String>()

        val tokenizer = ExpressionTokenizer(expr, functionTokens)
        for (token in tokenizer.getTokens()) {
            when (token.text) {
                "-", "+", "*", "/", in functionTokens -> {
                    while (operatorStack.isNotEmpty()
                            && getPrecedence(operatorStack.last()) >= getPrecedence(token.text)) {
                        addOperator(operatorStack, expressionStack)
                    }
                    operatorStack.add(token.text)
                }
                "—" -> {
                    operatorStack.add(token.text)
                }
                "(" -> operatorStack.add(token.text)
                ")" -> {
                    while (operatorStack.lastOrNull() != "(") {
                        addOperator(operatorStack, expressionStack)
                    }
                    operatorStack.safePop()
                    if (operatorStack.last() in functionTokens) {
                        addOperator(operatorStack, expressionStack)
                    }
                }
                else -> {
                    val value = token.text.toDouble()
                    var sign = 1
                    while (operatorStack.lastOrNull() == "—") {
                        sign *= -1
                        operatorStack.pop()
                    }
                    expressionStack.add(Value(sign * value))
                }
            }
        }

        while (operatorStack.isNotEmpty()) {
            if (operatorStack.last() == "(" || operatorStack.last() == ")") {
                throw Exception("Mismatched parentheses")
            }
            addOperator(operatorStack, expressionStack)
        }
        return if (expressionStack.size == 1) {
            expressionStack.last().eval()
        } else {
            throw Exception("Parsing error")
        }
    }

    private fun getPrecedence(operator : String) : Int = when (operator) {
        "+" -> 2
        "-" -> 2
        "*" -> 3
        "/" -> 3
        "(" -> 1
        in functionTokens -> 4
        else -> throw Exception("Wrong operation type: " + operator)
    }

    private fun addOperator(operatorStack : ArrayList<String>, expressionStack : ArrayList<Expression>) {
        val operator = operatorStack.pop()
        if (operator in functionTokens) {
            if (expressionStack.size < 1) {
                throw Exception("Wrong number of arguments")
            }
            expressionStack.add(Function(operator, expressionStack.pop()))
            return
        }
        if (operator == "—") {
            expressionStack.add(Operator(Value(-1.0), "*", expressionStack.pop()))
            return
        }
        if (expressionStack.size < 2) {
            throw Exception("Wrong number of arguments")
        }
        val right = expressionStack.pop()
        val left = expressionStack.pop()
        expressionStack.add(Operator(left, operator, right))
    }
}
