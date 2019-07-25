#pragma once
#include "serialize.hpp"
#include "print.hpp"
#include "system.hpp"
#include "symbol.hpp"

#include <tuple>
#include <limits>
#include "lemon.hpp"
namespace lemon {
  /**
   *  Defines %CPP API for managing assets
   *  @addtogroup asset Asset CPP API
   *  @ingroup cpp_api
   *  @{
   */

   /**
    * @struct Stores information for owner of asset
    */

   struct asset {
      /**
       * The amount of the asset
       */
      int64_t      amount = 0;

      /**
       * The symbol name of the asset
       */
      symbol  symbol;

      /**
       * Maximum amount possible for this asset. It's capped to 2^62 - 1
       */
      static constexpr int64_t max_amount    = (1LL << 62) - 1;

      asset() {}

      /**
       * Construct a new asset given the symbol name and the amount
       *
       * @param a - The amount of the asset
       * @param s - The name of the symbol
       */
      asset( int64_t a, class symbol s )
      :amount(a),symbol{s}
      {
         lemon::check( is_amount_within_range(), "magnitude of asset amount must be less than 2^62" );
         lemon::check( symbol.is_valid(),        "invalid symbol name" );
      }

      /**
       * Check if the amount doesn't exceed the max amount
       *
       * @return true - if the amount doesn't exceed the max amount
       * @return false - otherwise
       */
      bool is_amount_within_range()const { return -max_amount <= amount && amount <= max_amount; }

      /**
       * Check if the asset is valid. %A valid asset has its amount <= max_amount and its symbol name valid
       *
       * @return true - if the asset is valid
       * @return false - otherwise
       */
      bool is_valid()const               { return is_amount_within_range() && symbol.is_valid(); }

      /**
       * Set the amount of the asset
       *
       * @param a - New amount for the asset
       */
      void set_amount( int64_t a ) {
         amount = a;
         lemon::check( is_amount_within_range(), "magnitude of asset amount must be less than 2^62" );
      }

      /**
       * Unary minus operator
       *
       * @return asset - New asset with its amount is the negative amount of this asset
       */
      asset operator-()const {
         asset r = *this;
         r.amount = -r.amount;
         return r;
      }

      /**
       * Subtraction assignment operator
       *
       * @param a - Another asset to subtract this asset with
       * @return asset& - Reference to this asset
       * @post The amount of this asset is subtracted by the amount of asset a
       */
      asset& operator-=( const asset& a ) {
         lemon::check( a.symbol == symbol, "attempt to subtract asset with different symbol" );
         amount -= a.amount;
         lemon::check( -max_amount <= amount, "subtraction underflow" );
         lemon::check( amount <= max_amount,  "subtraction overflow" );
         return *this;
      }

      /**
       * Addition Assignment  operator
       *
       * @param a - Another asset to subtract this asset with
       * @return asset& - Reference to this asset
       * @post The amount of this asset is added with the amount of asset a
       */
      asset& operator+=( const asset& a ) {
         lemon::check( a.symbol == symbol, "attempt to add asset with different symbol" );
         amount += a.amount;
         lemon::check( -max_amount <= amount, "addition underflow" );
         lemon::check( amount <= max_amount,  "addition overflow" );
         return *this;
      }

      /**
       * Addition operator
       *
       * @param a - The first asset to be added
       * @param b - The second asset to be added
       * @return asset - New asset as the result of addition
       */
      inline friend asset operator+( const asset& a, const asset& b ) {
         asset result = a;
         result += b;
         return result;
      }

      /**
       * Subtraction operator
       *
       * @param a - The asset to be subtracted
       * @param b - The asset used to subtract
       * @return asset - New asset as the result of subtraction of a with b
       */
      inline friend asset operator-( const asset& a, const asset& b ) {
         asset result = a;
         result -= b;
         return result;
      }

      /**
       * @brief Multiplication assignment operator, with a number
       *
       * @details Multiplication assignment operator. Multiply the amount of this asset with a number and then assign the value to itself.
       * @param a - The multiplier for the asset's amount
       * @return asset - Reference to this asset
       * @post The amount of this asset is multiplied by a
       */
      asset& operator*=( int64_t a ) {
         int128_t tmp = (int128_t)amount * (int128_t)a;
         lemon::check( tmp <= max_amount, "multiplication overflow" );
         lemon::check( tmp >= -max_amount, "multiplication underflow" );
         amount = (int64_t)tmp;
         return *this;
      }

      /**
       * Multiplication operator, with a number proceeding
       *
       * @brief Multiplication operator, with a number proceeding
       * @param a - The asset to be multiplied
       * @param b - The multiplier for the asset's amount
       * @return asset - New asset as the result of multiplication
       */
      friend asset operator*( const asset& a, int64_t b ) {
         asset result = a;
         result *= b;
         return result;
      }


      /**
       * Multiplication operator, with a number preceeding
       *
       * @param a - The multiplier for the asset's amount
       * @param b - The asset to be multiplied
       * @return asset - New asset as the result of multiplication
       */
      friend asset operator*( int64_t b, const asset& a ) {
         asset result = a;
         result *= b;
         return result;
      }

      /**
       * @brief Division assignment operator, with a number
       *
       * @details Division assignment operator. Divide the amount of this asset with a number and then assign the value to itself.
       * @param a - The divisor for the asset's amount
       * @return asset - Reference to this asset
       * @post The amount of this asset is divided by a
       */
      asset& operator/=( int64_t a ) {
         lemon::check( a != 0, "divide by zero" );
         lemon::check( !(amount == std::numeric_limits<int64_t>::min() && a == -1), "signed division overflow" );
         amount /= a;
         return *this;
      }

      /**
       * Division operator, with a number proceeding
       *
       * @param a - The asset to be divided
       * @param b - The divisor for the asset's amount
       * @return asset - New asset as the result of division
       */
      friend asset operator/( const asset& a, int64_t b ) {
         asset result = a;
         result /= b;
         return result;
      }

      /**
       * Division operator, with another asset
       *
       * @param a - The asset which amount acts as the dividend
       * @param b - The asset which amount acts as the divisor
       * @return int64_t - the resulted amount after the division
       * @pre Both asset must have the same symbol
       */
      friend int64_t operator/( const asset& a, const asset& b ) {
         lemon::check( b.amount != 0, "divide by zero" );
         lemon::check( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount / b.amount;
      }

      /**
       * Equality operator
       *
       * @param a - The first asset to be compared
       * @param b - The second asset to be compared
       * @return true - if both asset has the same amount
       * @return false - otherwise
       * @pre Both asset must have the same symbol
       */
      friend bool operator==( const asset& a, const asset& b ) {
         lemon::check( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount == b.amount;
      }

      /**
       * Inequality operator
       *
       * @param a - The first asset to be compared
       * @param b - The second asset to be compared
       * @return true - if both asset doesn't have the same amount
       * @return false - otherwise
       * @pre Both asset must have the same symbol
       */
      friend bool operator!=( const asset& a, const asset& b ) {
         return !( a == b);
      }

      /**
       * Less than operator
       *
       * @param a - The first asset to be compared
       * @param b - The second asset to be compared
       * @return true - if the first asset's amount is less than the second asset amount
       * @return false - otherwise
       * @pre Both asset must have the same symbol
       */
      friend bool operator<( const asset& a, const asset& b ) {
         lemon::check( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount < b.amount;
      }

      /**
       * Less or equal to operator
       *
       * @param a - The first asset to be compared
       * @param b - The second asset to be compared
       * @return true - if the first asset's amount is less or equal to the second asset amount
       * @return false - otherwise
       * @pre Both asset must have the same symbol
       */
      friend bool operator<=( const asset& a, const asset& b ) {
         lemon::check( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount <= b.amount;
      }

      /**
       * Greater than operator
       *
       * @param a - The first asset to be compared
       * @param b - The second asset to be compared
       * @return true - if the first asset's amount is greater than the second asset amount
       * @return false - otherwise
       * @pre Both asset must have the same symbol
       */
      friend bool operator>( const asset& a, const asset& b ) {
         lemon::check( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount > b.amount;
      }

      /**
       * Greater or equal to operator
       *
       * @param a - The first asset to be compared
       * @param b - The second asset to be compared
       * @return true - if the first asset's amount is greater or equal to the second asset amount
       * @return false - otherwise
       * @pre Both asset must have the same symbol
       */
      friend bool operator>=( const asset& a, const asset& b ) {
         lemon::check( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount >= b.amount;
      }

      /**
       * %asset to std::string
       *
       * @brief %asset to std::string
       */
      std::string to_string()const {
         int64_t p = (int64_t)symbol.precision();
         int64_t p10 = 1;
         bool negative = false;
         int64_t invert = 1;

         while( p > 0  ) {
            p10 *= 10; --p;
         }
         p = (int64_t)symbol.precision();

         char fraction[p+1];
         fraction[p] = '\0';

         if (amount < 0) {
            invert = -1;
            negative = true;
         }

         auto change = (amount % p10) * invert;

         for( int64_t i = p -1; i >= 0; --i ) {
            fraction[i] = (change % 10) + '0';
            change /= 10;
         }
         char str[p+32];
         const char* fmt = negative ? "-%lld.%s %s" : "%lld.%s %s";
         snprintf(str, sizeof(str), fmt,
               (int64_t)(amount/p10), fraction, symbol.code().to_string().c_str());
         return {str};
      }

      /**
       * %Print the asset
       *
       * @brief %Print the asset
       */
      void print()const {
         lemon::print(to_string());
      }

      EOSLIB_SERIALIZE( asset, (amount)(symbol) )
   };

  /**
   * @struct Extended asset which stores the information of the owner of the asset
   */
   struct extended_asset {
      /**
       * The asset
       */
      asset quantity;

      /**
       * The owner of the asset
       */
      name contract;

      /**
       * Get the extended symbol of the asset
       *
       * @return extended_symbol - The extended symbol of the asset
       */
      extended_symbol get_extended_symbol()const { return extended_symbol{ quantity.symbol, contract }; }

      /**
       * Default constructor
       */
      extended_asset() = default;

       /**
       * Construct a new extended asset given the amount and extended symbol
       */
      extended_asset( int64_t v, extended_symbol s ):quantity(v,s.get_symbol()),contract(s.get_contract()){}
      /**
       * Construct a new extended asset given the asset and owner name
       */
      extended_asset( asset a, name c ):quantity(a),contract(c){}

      /**
       * %Print the extended asset
       */
      void print()const {
         quantity.print();
         prints("@");
         printn(contract.value);
      }

       /**
       *  Unary minus operator
       *
       *  @return extended_asset - New extended asset with its amount is the negative amount of this extended asset
       */
      extended_asset operator-()const {
         return {-quantity, contract};
      }

      /**
       * @brief Subtraction operator
       *
       * @details Subtraction operator. This subtracts the amount of the extended asset.
       * @param a - The extended asset to be subtracted
       * @param b - The extended asset used to subtract
       * @return extended_asset - New extended asset as the result of subtraction
       * @pre The owner of both extended asset need to be the same
       */
      friend extended_asset operator - ( const extended_asset& a, const extended_asset& b ) {
         lemon::check( a.contract == b.contract, "type mismatch" );
         return {a.quantity - b.quantity, a.contract};
      }

      /**
       * @brief Addition operator
       *
       * @details Addition operator. This adds the amount of the extended asset.
       * @param a - The extended asset to be added
       * @param b - The extended asset to be added
       * @return extended_asset - New extended asset as the result of addition
       * @pre The owner of both extended asset need to be the same
       */
      friend extended_asset operator + ( const extended_asset& a, const extended_asset& b ) {
         lemon::check( a.contract == b.contract, "type mismatch" );
         return {a.quantity + b.quantity, a.contract};
      }

      /// Addition operator.
      friend extended_asset& operator+=( extended_asset& a, const extended_asset& b ) {
         lemon::check( a.contract == b.contract, "type mismatch" );
         a.quantity += b.quantity;
         return a;
      }

      /// Subtraction operator.
      friend extended_asset& operator-=( extended_asset& a, const extended_asset& b ) {
         lemon::check( a.contract == b.contract, "type mismatch" );
         a.quantity -= b.quantity;
         return a;
      }

      /// Less than operator
      friend bool operator<( const extended_asset& a, const extended_asset& b ) {
         lemon::check( a.contract == b.contract, "type mismatch" );
         return a.quantity < b.quantity;
      }


      /// Comparison operator
      friend bool operator==( const extended_asset& a, const extended_asset& b ) {
         return std::tie(a.quantity, a.contract) == std::tie(b.quantity, b.contract);
      }

      /// Comparison operator
      friend bool operator!=( const extended_asset& a, const extended_asset& b ) {
         return std::tie(a.quantity, a.contract) != std::tie(b.quantity, b.contract);
      }

      /// Comparison operator
      friend bool operator<=( const extended_asset& a, const extended_asset& b ) {
         lemon::check( a.contract == b.contract, "type mismatch" );
         return a.quantity <= b.quantity;
      }

      /// Comparison operator
      friend bool operator>=( const extended_asset& a, const extended_asset& b ) {
         lemon::check( a.contract == b.contract, "type mismatch" );
         return a.quantity >= b.quantity;
      }

      EOSLIB_SERIALIZE( extended_asset, (quantity)(contract) )
   };

/// @} asset type
}
