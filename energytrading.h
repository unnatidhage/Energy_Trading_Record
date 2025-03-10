#ifndef ENERGYTRADING_H
#define ENERGYTRADING_H

#include <stdio.h>
#include <stdlib.h>

typedef enum { FAILURE,SUCCESS} status_code;

typedef struct Time_tag
{
	int hours;
	int min;
	int sec;
} Time;

typedef struct Date_tag
{
	int day;
	int month;
	int year;
} Date;

typedef struct Timestamp_tag
{
	Date date;
	Time time;	
} Timestamp;


typedef struct Transaction_tag
{
	int sr_no;
	int TransactionID;
	int BuyerID;
	int SellerID;
	float EnergyAmount;
	float price_pkWh;
	Timestamp timestamp;
	struct Transaction_tag *next;
} Transaction;

typedef struct Transaction_ptr_tag
{
	Transaction *tptr;
	struct Transaction_ptr_tag *next;
}Transaction_ptr;


typedef struct Buyer_tag	
{
	int BuyerID;
	Transaction_ptr* t_lptr;
	int num_Trans;
	float Total_Energy;
	struct Buyer_tag *next;
} Buyer;

typedef struct Buyer_ptr_tag
{
	int BuyerID;
	struct Buyer_ptr_tag *next;
} Buyer_ptr;

typedef struct Seller_tag
{
	int Sr_n;
	int SellerID;
	float p_below_300kWh;
	float p_above_300kWh;
	Buyer_ptr* Regular_Buyers;
	Transaction_ptr  *t_lptr;
	struct Seller_tag *next;
} Seller;


typedef struct Seller_Buyer_Pair_tag
{
	int SellerID;
	int BuyerID;
	Transaction_ptr *t_lptr;
	int num_Trans;
	struct Seller_Buyer_Pair_tag *next;
} Seller_Buyer_Pair;

Transaction* Create_Transaction_Node (int sr_no, int T_ID, int B_ID, int S_ID, float E_Amount, float price_pkWh, Timestamp timestamp);

void Create_Transaction_List(FILE* file , Transaction** first_tptr, Transaction** last_tptr);

void Create_Buyer_List(Transaction *first_t, Buyer **first_bptr, Buyer **last_bptr);

void Create_Seller_List( FILE* file, Transaction* first_t, Seller** first_sptr, Seller** last_sptr);

void Add_Transaction_List_Seller(Seller *first_s, Transaction *first_t );

void Add_Regular_Buyer_List(Seller_Buyer_Pair *first_sb,Seller *first_s);

void Create_Seller_Buyer_Pair_List(Transaction *first_t, Seller_Buyer_Pair **first_sbptr, Seller_Buyer_Pair **last_sbptr);

Timestamp Input_TimeStamp();

status_code Add_New_Transaction(FILE *file1, FILE *file2, Transaction **last_tptr,Buyer *first_b,Buyer **last_bptr, Seller *first_s, Seller **last_sptr,int T_ID, int B_ID, int S_ID, float E_Amount, Timestamp timestamp);

void Display_Transactions(Transaction *first_t);

void Display_List_Transaction_Seller(Seller *first_s);

void Display_List_Transaction_Buyer(Buyer *first_b);

float TotalRevenue (int S_ID, Seller *first_s);

void Update_Energy(Transaction *t_ptr , Buyer *b_ptr, Seller *s_ptr, float energy);

void Update_Price(Transaction *t_ptr, Seller *s_ptr,float price);

void Update_Timestamp(Transaction *t_ptr, Timestamp t);

void UpdateTransaction (FILE *file1, FILE *file2, int T_ID, Transaction *first_t, Seller *first_s, Buyer *first_b);

Transaction * Divide_t(Transaction * first_t);

Buyer* Divide_b(Buyer *first_b);

Seller_Buyer_Pair* Divide_sb(Seller_Buyer_Pair *first_sb);

Transaction* Merge_t(Transaction *list1, Transaction *list2);

Buyer* Merge_b(Buyer *list1, Buyer *list2);

Seller_Buyer_Pair* Merge_sb(Seller_Buyer_Pair *list1, Seller_Buyer_Pair *list2);

Transaction* MergeSort_dec_t(Transaction *first_t);

void Set_Last_t(Transaction *first_t,Transaction **last_tptr);

void Print_dec_Sorted_Transaction(Transaction *first_t);

Buyer* MergeSort_inc_b(Buyer *first_b);

void Set_Last_b(Buyer *first_b,Buyer **last_bptr);

void Print_inc_Sorted_Buyer(Buyer *first_b);

Seller_Buyer_Pair* MergeSort_dec_sb(Seller_Buyer_Pair *first_t);

void Set_Last_sb(Seller_Buyer_Pair *first_sb,Seller_Buyer_Pair **last_sbptr);

void Print_dec_Sorted_SellerBuyer(Seller_Buyer_Pair *first_sb);

int isLeapYear(int year);

long calculateDays(Date d);

long Calculate_sec(Timestamp t);

void Transactions_in_timeperiod(Timestamp t1, Timestamp t2, Transaction *first_t);

#endif






