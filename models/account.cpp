#include "account.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace NickvisionMoney::Models
{
    Account::Account(const std::string& path) : m_path(path), m_db(m_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
    {
        m_db.exec("CREATE TABLE IF NOT EXISTS transactions (id INTEGER PRIMARY KEY, date TEXT, description TEXT, type INTEGER, amount REAL)");
        SQLite::Statement qryGetAll(m_db, "SELECT * FROM transactions");
        while(qryGetAll.executeStep())
        {
            Transaction transaction(qryGetAll.getColumn(0).getInt());
            transaction.setDate(qryGetAll.getColumn(1).getString());
            transaction.setDescription(qryGetAll.getColumn(2).getString());
            transaction.setType(static_cast<TransactionType>(qryGetAll.getColumn(3).getInt()));
            transaction.setAmount(qryGetAll.getColumn(4).getDouble());
            m_transactions.insert({ transaction.getID(), transaction });
        }
    }

    const std::string& Account::getPath() const
    {
        return m_path;
    }

    const std::map<unsigned int, Transaction>& Account::getTransactions() const
    {
        return m_transactions;
    }

    std::optional<Transaction> Account::getTransactionByID(unsigned int id) const
    {
        try
        {
            return m_transactions.at(id);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    int Account::getNextID() const
    {
        if(m_transactions.empty())
        {
            return 1;
        }
        else
        {
            return m_transactions.rbegin()->first + 1;
        }
    }

    bool Account::addTransaction(const Transaction& transaction)
    {
        SQLite::Statement qryInsert(m_db, "INSERT INTO transactions (id, date, description, type, amount) VALUES (?, ?, ?, ?, ?)");
        qryInsert.bind(1, transaction.getID());
        qryInsert.bind(2, transaction.getDate());
        qryInsert.bind(3, transaction.getDescription());
        qryInsert.bind(4, static_cast<int>(transaction.getType()));
        qryInsert.bind(5, transaction.getAmount());
        if(qryInsert.exec() > 0)
        {
           m_transactions.insert({ transaction.getID(), transaction });
            return true;
        }
        return false;
    }

    bool Account::updateTransaction(const Transaction& transaction)
    {
        SQLite::Statement qryUpdate(m_db, "UPDATE transactions SET date = ?, description = ?, type = ?, amount = ? WHERE id = " + std::to_string(transaction.getID()));
        qryUpdate.bind(1, transaction.getDate());
        qryUpdate.bind(2, transaction.getDescription());
        qryUpdate.bind(3, static_cast<int>(transaction.getType()));
        qryUpdate.bind(4, transaction.getAmount());
        if(qryUpdate.exec() > 0)
        {
            m_transactions[transaction.getID()] = transaction;
            return true;
        }
        return false;
    }

    bool Account::deleteTransaction(unsigned int id)
    {
       if(m_db.exec("DELETE FROM transactions WHERE id = " + std::to_string(id)) > 0)
       {
           m_transactions.erase(id);
           return true;
       }
       return false;
    }

    double Account::getIncome() const
    {
        double income = 0.00;
        for(const std::pair<unsigned int, Transaction>& pair : m_transactions)
        {
            if(pair.second.getType() == TransactionType::Income)
            {
                income += pair.second.getAmount();
            }
        }
        return income;
    }

    std::string Account::getIncomeAsString() const
    {
        std::stringstream builder;
        builder << std::fixed << std::setprecision(2) << std::ceil(getIncome() * 100.0) / 100.0;
        return builder.str();
    }

    double Account::getExpense() const
    {
        double expense = 0.00;
        for(const std::pair<unsigned int, Transaction>& pair : m_transactions)
        {
            if(pair.second.getType() == TransactionType::Expense)
            {
                expense += pair.second.getAmount();
            }
        }
        return expense;
    }

    std::string Account::getExpenseAsString() const
    {
        std::stringstream builder;
        builder << std::fixed << std::setprecision(2) << std::ceil(getExpense() * 100.0) / 100.0;
        return builder.str();
    }

    double Account::getTotal() const
    {
        double total = 0.00;
        for(const std::pair<unsigned int, Transaction>& pair : m_transactions)
        {
            if(pair.second.getType() == TransactionType::Income)
            {
                total += pair.second.getAmount();
            }
            else
            {
                total -= pair.second.getAmount();
            }
        }
        return total;
    }

    std::string Account::getTotalAsString() const
    {
        std::stringstream builder;
        builder << std::fixed << std::setprecision(2) << std::ceil(getTotal() * 100.0) / 100.0;
        return builder.str();
    }

    void Account::backup(const std::string& backupPath)
    {
        if(backupPath == m_path)
        {
            throw std::invalid_argument("Backup path can not be the same as the open database's path.");
        }
        m_db.backup(backupPath.c_str(), SQLite::Database::BackupType::Save);
    }

    void Account::restore(const std::string& restorePath)
    {
        if(restorePath == m_path)
        {
            throw std::invalid_argument("Restore path can not be the same as the open database's path.");
        }
        m_db.backup(restorePath.c_str(), SQLite::Database::BackupType::Load);
        m_transactions.clear();
        SQLite::Statement qryGetAll(m_db, "SELECT * FROM transactions");
        while(qryGetAll.executeStep())
        {
            Transaction transaction(qryGetAll.getColumn(0).getInt());
            transaction.setDate(qryGetAll.getColumn(1).getString());
            transaction.setDescription(qryGetAll.getColumn(2).getString());
            transaction.setType(static_cast<TransactionType>(qryGetAll.getColumn(3).getInt()));
            transaction.setAmount(qryGetAll.getColumn(4).getDouble());
            m_transactions.insert({ transaction.getID(), transaction });
        }
    }
}
