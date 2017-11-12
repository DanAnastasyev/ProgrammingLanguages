import kotlin.js.Math

interface Expression {
    fun eval() : Double
}

class Value(private val value : Double) : Expression {
    override fun eval() = value
}

class StringExpression(val text : String) : Expression {
    override fun eval() = throw Exception("Not supported operation StringExpression::eval()")
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
        "LEN" -> (arg as StringExpression).text.length.toDouble()
        else -> throw Exception("Wrong operation type")
    }
}

interface Table {
    fun getCellText(rowInd : Int, colInd : Char) : String
}

class ExpressionParser(private val table : Table) {
    private val functionTokens = hashSetOf("ABS", "SIN", "LEN")
    private val numberRegex = Regex("([0-9]+)|([0-9]*\\.[0-9]+)")

    fun parse(expr : String) : Double {
        val expressionStack = ArrayList<Expression>()
        val operatorStack = ArrayList<Token>()

        val tokenizer = ExpressionTokenizer(expr, functionTokens)
        for (token in tokenizer.getTokens()) {
            when (token) {
                is OperatorToken -> handleOperatorToken(token, operatorStack, expressionStack)
                is FunctionToken -> handleStandardOperationToken(token, operatorStack, expressionStack)
                is NumberToken -> handleNumberToken(token, operatorStack, expressionStack)
                is CellToken -> handleCellToken(token, expressionStack)
                is StringToken -> expressionStack.add(StringExpression(token.text))
            }
        }

        while (operatorStack.isNotEmpty()) {
            if (operatorStack.last() == openParenthesis || operatorStack.last() == closeParenthesis) {
                throw Exception("Mismatched parentheses")
            }
            processOperator(operatorStack.pop(), expressionStack)
        }

        if (expressionStack.size == 1) {
             return expressionStack.last().eval()
        } else {
            throw Exception("Parsing error")
        }
    }

    private fun handleOperatorToken(token: OperatorToken, operatorStack: ArrayList<Token>,
                                    expressionStack: ArrayList<Expression>) {
        when (token.operation) {
            '-', '+', '*', '/' -> handleStandardOperationToken(token, operatorStack, expressionStack)
            '—', '(' -> operatorStack.add(token)
            ')' -> handleCloseParenthesis(operatorStack, expressionStack)
        }
    }

    private fun handleCloseParenthesis(operatorStack: ArrayList<Token>, expressionStack: ArrayList<Expression>) {
        while (operatorStack.lastOrNull() != openParenthesis) {
            processOperator(operatorStack.pop(), expressionStack)
        }
        operatorStack.safePop()
        if (operatorStack.last() is FunctionToken) {
            processOperator(operatorStack.pop(), expressionStack)
        }
    }

    private fun handleStandardOperationToken(token : Token, operatorStack : ArrayList<Token>,
                                             expressionStack : ArrayList<Expression>) {
        while (operatorStack.isNotEmpty()
                && getPrecedence(operatorStack.last()) >= getPrecedence(token)) {
            processOperator(operatorStack.pop(), expressionStack)
        }
        operatorStack.add(token)
    }

    private fun processOperator(operator : Token, expressionStack : ArrayList<Expression>) {
        when (operator) {
            is FunctionToken -> addFunctionExpression(operator, expressionStack)
            is OperatorToken -> addOperatorExpression(operator, expressionStack)
        }
    }

    private fun addOperatorExpression(operator : OperatorToken, expressionStack : ArrayList<Expression>) {
        when (operator) {
            unaryMinus -> {
                if (expressionStack.size < 1) {
                    throw Exception("Cannot find argument for unary minus")
                }
                expressionStack.add(Operator(Value(-1.0), '*', expressionStack.pop()))
            }
            else -> {
                if (expressionStack.size < 2) {
                    throw Exception("Wrong number of arguments")
                }
                val right = expressionStack.pop()
                val left = expressionStack.pop()
                expressionStack.add(Operator(left, operator.operation, right))
            }
        }
    }

    private fun addFunctionExpression(operator : FunctionToken, expressionStack : ArrayList<Expression>) {
        if (expressionStack.size < 1) {
            throw Exception("Wrong number of arguments")
        }
        expressionStack.add(Function(operator.text, expressionStack.pop()))
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

    private fun handleCellToken(token: CellToken, expressionStack: ArrayList<Expression>) {
        val otherCellText = table.getCellText(token.rowIndex, token.columnIndex)
        if (otherCellText.startsWith('=')) {
            expressionStack.add(Value(parse(otherCellText.substring(1))))
        } else if (numberRegex.matches(otherCellText)) {
            expressionStack.add(Value(otherCellText.toDouble()))
        } else {
            expressionStack.add(StringExpression(otherCellText))
        }
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
