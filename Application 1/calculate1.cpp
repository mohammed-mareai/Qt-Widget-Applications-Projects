#include "calculate1.h"

Calculate1::Calculate1(QObject *parent)
    : QObject{parent}
{}

/*
 This function will break down the operation into two list one list for operators {+, -, ...etc}. The second List is
 for numbers being calcualted.
 The function works as follow:
    1. Iterate throug the string and seprate the operators and numbers
    2. Merge the negative sign string value with the number
    3. Convert the numbers to double to add them to values
    4. If numbers and operator are not equal in size. Add "+" to the begening indicating positive value

 */
void Calculate1::parseExpression(const QString &expression, QList<QString> &operators, QList<double> &values)
{
    // Regular expression to split the string while keeping the operators
    QRegularExpression re("([\\d\\.]+|[*\\-/+])");
    QRegularExpressionMatchIterator i = re.globalMatch(expression);

    QList<QString> tokens;
    // Iterate through the string and extract tokens
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        tokens << match.captured(1);
    }

    // Process the tokens to merge negative sign with the number
    QList<QString> processedTokens;
    for (int j = 0; j < tokens.size(); ++j) {
        if (tokens[j] == "-" && j > 0 && (tokens[j-1] == "+" || tokens[j-1] == "-" || tokens[j-1] == "*" || tokens[j-1] == "/")) {
            // Merge '-' with the next number
            if (j + 1 < tokens.size()) {
                processedTokens.removeLast(); // Remove the operator before '-'
                processedTokens << tokens[j-1];
                processedTokens << tokens[j] + tokens[j+1];
                ++j; // Skip the next number as it is already merged
            }
        } else {
            processedTokens << tokens[j];
        }
    }

    // Iterate through the tokens
    for (const QString &token : processedTokens) {
        // Check if the token is a number
        bool ok;
        double value = token.toDouble(&ok);
        if (ok) {
            // If it's a number, append to values list
            values.append(value);
        } else {
            // If it's an operator, append to operators list
            operators.append(token);
        }
    }

    // if both lists are not equal, add "+" in the begening to show positive value in the begening
    if (operators.size() != values.size()) {
        operators.insert(0, "+");
    }
}

/*
 This function will take the list of operators and values (numbers) and will reduce the negative and division.
 This function works as follow:
    1. It keeps "+" and "*" the same
    2. It convets "-" to "+" by multiplying by -1
    3. It converts "/" to "*" by dividing 1/(value)
 */
void Calculate1::reduceSigns(QList<QString> &op, QList<double> &vals)
{
    for (int i = 0; i < vals.size(); ++i) {
        if (op[i] == "+" || op[i] == "*") {
            // No change needed for addition and multiplication
            vals[i] = vals[i] * 1;
        } else if (op[i] == "-") {
            // Negate the value for subtraction
            vals[i] = vals[i] * -1;
            op[i] = "+";
        } else if (op[i] == "/") {
            // Invert the value for division
            vals[i] = 1 / vals[i];
            op[i] = "*";
        }
    }
}
/*
 This function will make sure that multiplication (division has been reduced) is being performed before addition
 The functions works as follow:
    1. It extract the indicies of "*" operator to multiplyIndices
    2. Extract the numbers that will be multiplied and attach them to currentSequence
    3. Append the first index of numbers being multiplied with current Sequence inside QMap<int, QList<double>>
    4. Repeat step 2 and 3 until there is no numbers to be multiplied
    5. Subtract One from all indices inside the sequence QMap
 */

void Calculate1::MapForMultiplication(QMap<int, QList<double>> &sequences, QList<QString> &op, QList<double> &vals)
{
    // extract the indices for multiplication * signs
    QList<int> multiplyIndices;
    for (int i = 0; i < op.size(); ++i) {
        if (op[i] == "*") {
            multiplyIndices.append(i);
        }
    }

    QList<double> currentSequence;
    int startIdx = -1;

    for (int i = 0; i < multiplyIndices.size(); ++i) {
        int currentIndex = multiplyIndices[i];
        if (i == 0 || currentIndex != multiplyIndices[i - 1] + 1) {
            // Start of a new sequence
            if (!currentSequence.isEmpty()) {
                // If there's a previous sequence, append it to the map
                sequences.insert(startIdx, currentSequence);
                currentSequence.clear();
            }
            currentSequence.append(vals[currentIndex-1]);
            currentSequence.append(vals[currentIndex]);
            startIdx = currentIndex;
        } else {
            // Continue the current sequence

            currentSequence.append(vals[currentIndex]);
        }
    }

    // Append the last sequence if any
    if (!currentSequence.isEmpty()) {
        sequences.insert(startIdx, currentSequence);
    }

    // Get the list of keys
    QList<int> keys = sequences.keys();

    // Subtract 1 from each key and update the QMap
    for (int i = 0; i < keys.size(); ++i) {
        int oldKey = keys[i];
        int newKey = oldKey - 1;

        // Retrieve the value associated with the old key
        QList<double> value = sequences.take(oldKey);

        // Insert the value with the new key
        sequences.insert(newKey, value);
    }
}

/*
 This function will multiply the values in the QMap and append the result to the operation.
 It works as follow:
    1. Convert the QMap<int, QList<double>> to QMap<int, double> by mutliplying the QList of double values
    2. Check the Qlist values from QList<double> inside the QMap and replace them with the result step 1
    The code tries to make sure that the values present in sequence. For example if QMap((2, QList(2, 3))), it will check that
    vals have 2, 3 in sequece then replace them
 */

void Calculate1::multiplyResults(const QMap<int, QList<double> > &sequences, const QList<double> &vals, QList<double> &NewVals)
{
    QMap<int, double> resultMap;

    for (auto it = sequences.begin(); it != sequences.end(); ++it) {
        int key = it.key();
        double product = 1.0;
        for (double value : it.value()) {
            product *= value;
        }
        resultMap.insert(key, product);
    }

    // Iterate over the original vals list
    for (int i = 0; i < vals.size(); ++i) {
        bool found = false;
        // Check if the value at index i is present in any of the sequences
        for (auto it = sequences.constBegin(); it != sequences.constEnd(); ++it) {
            if (it.key() <= i && i < it.key() + it.value().size()) {
                found = true;
                break;
            }
        }
        // If the value is not present in any sequence, add it to NewVals
        if (!found) {
            NewVals.append(vals[i]);
        }
    }

    // Iterate over resultMap and add values to NewVals
    for (auto it = resultMap.constBegin(); it != resultMap.constEnd(); ++it) {
        NewVals.append(it.value());
    }
}

/*
 This function will give the sum of the values since multiplicaiton is already done before
 */
double Calculate1::sumValues(const QList<double> &values)
{
    double sum = 0.0;
    for (double value : values) {
        sum += value;
    }
    return sum;
}

// this function is just a collection of all previous functions before from taking the input operation to giving the result
double Calculate1::BasicOpCalc(const QString operation)
{
    QList <QString> op;
    QList <double> vals;
    parseExpression(operation, op, vals);
    reduceSigns(op, vals);
    QMap<int, QList<double> > sequences;
    MapForMultiplication(sequences, op, vals);
    QList <double> NewVals;
    multiplyResults(sequences, vals, NewVals);

    return sumValues(NewVals);
}

// ************************************************************** Solve for Brackets **********************************************
// this function to check if the input has Brackets
bool Calculate1::hasBrackets(const QString &str) {
    for (int i = 0; i < str.length(); ++i) {
        if (str[i] == '(' || str[i] == ')') {
            return true;
        }
    }
    return false;
}

// this function is to check if left and right brackets are equal
bool Calculate1::bracketsBalanced(const QString &str) {
    int leftCount = 0;
    int rightCount = 0;

    for (int i = 0; i < str.length(); ++i) {
        if (str[i] == '(') {
            leftCount++;
        } else if (str[i] == ')') {
            rightCount++;
        }
    }

    return leftCount == rightCount;
}

/*
 This function will deal with bracket and calculate them using the function defined before BasicOpCalc(const QString operation).
 This function works as follow:
    1. Obtain the index of "(" and ")" in the string. This is done by:
        1.1 Check the index of the first "(" from the left
        1.2 If there is another "(", nested brackets it will update to the next one
        1.3 when the ")" is found it will save the last index found for "(" with ")" index inside QPair <int, int> indices;
    2. Take the operation inside the brackes using their indices and calcualte it using BasicOpCalc().
    3. Replace the brackets with the result
    4. Repeat steps 1-3 as much bracket count (based on the number of brackets exist.
 */
QString Calculate1::BracketsOpCalc(QString op)
{
    int count = 0;
    for (int i = 0; i < op.length(); ++i) {
        if (op[i] == '(') {
            count++;
        }
    }

    for(int i = 0; i < count; ++i){
        std::stack<int> bracketIndices;
        QPair <int, int> indices;
        for (int i = 0; i < op.length(); ++i) {
            if (op[i] == '(') {
                bracketIndices.push(i);
            } else if (op[i] == ')') {
                if (!bracketIndices.empty()) {
                    int startIndex = bracketIndices.top();
                    bracketIndices.pop();

                    indices.first = startIndex;
                    indices.second = i;
                    break;

                } else {
                    qDebug() << "Error: Unexpected ')' at index " << i << ", no matching '(' found." ;
                }
            }
        }

        QString TempOp = op.mid(indices.first+1, indices.second-indices.first-1);
        double result = BasicOpCalc(TempOp);
        QString strResult = QString::number(result);
        op.replace(indices.first, indices.second - indices.first + 1, strResult);
    }

    return op;
}
