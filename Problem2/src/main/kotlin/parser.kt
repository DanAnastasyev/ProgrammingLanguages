import kotlin.js.Math

interface Expression {
    fun eval() : Double
}

class Value(private val value : Double) : Expression {
    override fun eval() = value
}

class Operator(private val left : Expression, private val operation : Char,
               private val right : Expression) : Expression {

    override fun eval() : Double = when (operation) {
        '+' -> left.eval() + right.eval()
        '-' -> left.eval() - right.eval()
        '*' -> left.eval() * right.eval()
        '/' -> left.eval() / right.eval()
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

interface Table {
    fun getCellText(rowInd : Int, colInd : Char) : String
}

class ExpressionParser(private val table : Table) {
    private val functionTokens = hashSetOf("ABS", "SIN", "LEN")

    fun parse(expr : String) : Double {
        val expressionStack = ArrayList<Expression>()
        val operatorStack = ArrayList<Token>()

        val tokenizer = ExpressionTokenizer(expr, functionTokens)
        for (token in tokenizer.getTokens()) {
            when (token) {
                is OperatorToken -> handleOperatorToken(token, operatorStack, expressionStack)
                is FunctionToken -> handleStandardOperationToken(token, operatorStack, expressionStack)
                is NumberToken -> handleNumberToken(token, operatorStack, expressionStack)
                is CellToken -> {
                    val otherCellText = table.getCellText(token.rowIndex, token.columnIndex)
                    if (otherCellText.startsWith('=')) {
                        expressionStack.add(Value(parse(otherCellText.substring(1))))
                    } else {
                        expressionStack.add(Value(otherCellText.toDouble()))
                    }
                }
            }
        }

        while (operatorStack.isNotEmpty()) {
            if (operatorStack.last() == openParenthesis || operatorStack.last() == closeParenthesis) {
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

    private fun handleOperatorToken(token: OperatorToken, operatorStack: ArrayList<Token>,
                                    expressionStack: ArrayList<Expression>) {
        when (token.operation) {
            '-', '+', '*', '/' -> handleStandardOperationToken(token, operatorStack, expressionStack)
            '—', '(' -> operatorStack.add(token)
            ')' -> {
                while (operatorStack.lastOrNull() != openParenthesis) {
                    addOperator(operatorStack, expressionStack)
                }
                operatorStack.safePop()
                if (operatorStack.last() is FunctionToken) {
                    addOperator(operatorStack, expressionStack)
                }
            }
        }
    }

    private fun handleStandardOperationToken(token : Token, operatorStack : ArrayList<Token>,
                                             expressionStack : ArrayList<Expression>) {
        while (operatorStack.isNotEmpty()
                && getPrecedence(operatorStack.last()) >= getPrecedence(token)) {
            addOperator(operatorStack, expressionStack)
        }
        operatorStack.add(token)
    }

    private fun addOperator(operatorStack : ArrayList<Token>, expressionStack : ArrayList<Expression>) {
        val operator = operatorStack.pop()
        when (operator) {
            is FunctionToken -> {
                if (expressionStack.size < 1) {
                    throw Exception("Wrong number of arguments")
                }
                expressionStack.add(Function(operator.text, expressionStack.pop()))
            }
            is OperatorToken -> {
                if (operator == unaryMinus) {
                    expressionStack.add(Operator(Value(-1.0), '*', expressionStack.pop()))
                    return
                }
                if (expressionStack.size < 2) {
                    throw Exception("Wrong number of arguments")
                }
                val right = expressionStack.pop()
                val left = expressionStack.pop()
                expressionStack.add(Operator(left, operator.operation, right))
            }
        }
    }

    private fun handleNumberToken(token : NumberToken, operatorStack : ArrayList<Token>,
                                  expressionStack : ArrayList<Expression>) {
        var sign = 1
        while (operatorStack.lastOrNull() == unaryMinus) {
            sign *= -1
            operatorStack.pop()
        }
        expressionStack.add(Value(sign * token.value))
    }

    private fun getPrecedence(token : Token) : Int = when (token) {
        is OperatorToken -> when (token.operation) {
            '(' -> 1
            '+', '-' -> 2
            '*', '/' -> 3
            else -> throw Exception("Unknown operation type: " + token.operation)
        }
        is FunctionToken -> 4
        else -> throw Exception("Unknown operation type")
    }
}
