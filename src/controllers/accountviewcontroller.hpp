#pragma once

#include <functional>
#include <map>
#include <string>
#include "transactiondialogcontroller.hpp"
#include "../models/account.hpp"
#include "../models/transaction.hpp"

namespace NickvisionMoney::Controllers
{
	/**
	 * A controller for the AccountView
	 */
	class AccountViewController
	{
	public:
		/**
		 * Constructs an AccountViewController
		 *
		 * @param path The path of the account to open
		 * @param currencySymbol The symbol to use when displaying monetary values
		 */
		AccountViewController(const std::string& path, const std::string& currencySymbol);
		/**
		 * Gets the path of the account
		 *
		 * @returns The path of the account
		 */
		const std::string& getAccountPath() const;
		/**
		 * Gets the symbol to use when displaying monetary values
		 *
		 * @returns The symbol to use when displaying monetary values
		 */
		const std::string& getCurrencySymbol() const;
		/**
		 * Gets the total of the account as a string
		 *
		 * @returns The total of the account
		 */
		std::string getAccountTotalString() const;
		/**
		 * Gets the income of the account as a string
		 *
		 * @returns The income of the account
		 */
		std::string getAccountIncomeString() const;
		/**
		 * Gets the expense of the account as a string
		 *
		 * @returns The expense of the account
		 */
		std::string getAccountExpenseString() const;
		/**
		 * Gets the map of transactions in the account
		 *
		 * @returns The transaction in the account
		 */
		const std::map<unsigned int, NickvisionMoney::Models::Transaction>& getTransactions() const;
		/**
		 * Registers a callback for updating the UI when an account's info is changed
		 *
		 * @param callback A void() function
		 */
		void registerAccountInfoChangedCallback(const std::function<void()>& callback);
		/**
		 * Adds a new transaction to the account
		 *
		 * @param transaction The transaction to add
		 */
		void addTransaction(const NickvisionMoney::Models::Transaction& transaction);
		/**
		 * Updates a transaction in the account
		 *
		 * @param transaction The transaction to update
		 */
		void updateTransaction(const NickvisionMoney::Models::Transaction& transaction);
		/**
		 * Deletes a transaction in the account
		 *
		 * @param id The id of the transaction to delete
		 */
		void deleteTransaction(unsigned int id);
		/**
		 * Creates a TransactionDialogController for a new transaction
		 *
		 * @returns A new TransactionDialogController
		 */
		TransactionDialogController createTransactionDialogController() const;
		/**
		 * Creates a TransactionDialogController for an existing transaction
		 *
		 * @param id The id of the transaction to edit
		 * @returns A new TransactionDialogController
		 */
		TransactionDialogController createTransactionDialogController(unsigned int id) const;

	private:
		std::string m_currencySymbol;
		NickvisionMoney::Models::Account m_account;
		std::function<void()> m_accountInfoChangedCallback;
	};
}