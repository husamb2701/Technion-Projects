#include <string>
#include <fstream>
using namespace std;
using std::string;
using std::ofstream;
#include "Utilities.h"
#include "Transaction.h"


/**
*
 * Transaction - Defining the new Transaction Type
 *
*/


/**
 * TransactionDumpInfo - Prints the data of the transaction to a given file
 *
 * The data is printed in the following format:
 * Sender Name: <name>
 * Receiver Name: <name>
 * Transaction Value: <value>
 *
 * @param transaction Transaction to print
*/
void TransactionDumpInfo(const Transaction& transaction, ofstream& file) {
 if (!file) {
  return;
 }
 file << "Sender Name: <" << transaction.sender << ">\n";
 file << "Receiver Name: <" << transaction.receiver << ">\n";
 file << "Transaction Value: <" << transaction.value << ">\n";
}


/**
 * TransactionHashMessage - Hashs the message of the transaction
 *
 * @param transaction Transaction to hash
 *
 * @return The hashed message
*/
string TransactionHashedMessage(const Transaction& transaction) {
 return :: hash(transaction.value, transaction.sender,
     transaction.receiver);
}

/**
 * TransactionVerifyHashedMessage - Verifies that a given transaction suits a given hashed message
 *
 * @param transaction Given transaction
 * @param hashedMessage Hashed message to verify
 *
 * @return true if the message given is suitable to this transaction, false otherwise
 *
*/
bool TransactionVerifyHashedMessage(const Transaction& transaction, string hashedMessage) {
 if (TransactionHashedMessage(transaction) == hashedMessage) {
  return true;
 }else {
  return false;
 }
}
