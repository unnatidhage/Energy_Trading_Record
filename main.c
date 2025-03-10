#include "energytrading.h"

int main()
{
	printf("Program starting...\n"); //
	const char* Transaction_filename = "Energy_Transactions.txt";
	const char* Seller_filename = "Seller_Details.txt";
	
	printf("Opening transaction file: %s\n", Transaction_filename); //
	FILE* file1 = fopen(Transaction_filename,"r+");
	if (file1 == NULL)
	{
		printf("Error opening file: %s\n",Transaction_filename);
	}
    printf("Opening Seller file\n"); //
	FILE* file2 = fopen(Seller_filename,"r+");
	if(file2 == NULL)
	{
		printf("Error opening file: %s\n",Seller_filename);
	}
	if(file1 != NULL && file2 != NULL)
	{
		printf("Successfully opened transaction file\n"); //
		printf("Seller file created successfully\n");//
	    Transaction *first_t , *last_t;
		first_t=last_t=NULL;
		Create_Transaction_List(file1,&first_t,&last_t);
		//
		if(first_t != NULL){
			printf("Transaction list created successfully\n");
		}
		//
		
		Buyer *first_b , *last_b;
		first_b=last_b=NULL;
		Create_Buyer_List(first_t,&first_b,&last_b)	;
		//
		if(first_b != NULL)
		{
			printf("Buyer list created successfulyy\n");
		}
		//
		
			
		Seller *first_s, *last_s;
		first_s=last_s=NULL;
		Create_Seller_List(file2,first_t,&first_s,&last_s);
		
		//
		if(first_s!=NULL)
		{
			printf("Seller list created successfully\n");				
		}
		//
		Seller_Buyer_Pair *first_sb, *last_sb;
		first_sb=last_sb=NULL;
		Create_Seller_Buyer_Pair_List(first_t,&first_sb,&last_sb);
		//
		if(first_sb!=NULL)
		{
			printf("Seller-Buyer list created successfully\n");
		}
	    Add_Regular_Buyer_List(first_sb,first_s);
		Add_Transaction_List_Seller(first_s,first_t);
			
		//Add New Transaction
		printf("\n");
		printf("Enter the details of the new transaction to be added:\n");
		int T_ID, B_ID, S_ID;
		float E_Amount;
		printf("Transaction ID: ");
		scanf("%d",&T_ID);
		printf("\nBuyer ID: ");
		scanf("%d",&B_ID);
		printf("\nSeller ID: ");
		scanf("%d",&S_ID);
		printf("\nEnergy Amount: ");
		scanf("%f",&E_Amount);
		Timestamp t = Input_TimeStamp();
		status_code sc = Add_New_Transaction(file1,file2,&last_t,first_b,&last_b,first_s,&last_s,T_ID,B_ID,S_ID,E_Amount,t);
		if(sc)
		{
			printf("Addition of new transaction has been successful.\n");
		}
		else
		{
			printf("New Transaction couldn't be added.\n");
		}
			
		printf("\n");
		//Display All Transactions
		Display_Transactions(first_t);
		    
		printf("\n");
		//Create Transaction List for every Seller
		Display_List_Transaction_Seller(first_s);
		    
		printf("\n");
		//Create Transcation List for every Buyer
		Display_List_Transaction_Buyer(first_b);
		    
		printf("\n");
		//List of Transactions that took place in a given time period.
		printf("Enter the timestamp for beginning of time period:\n");
		Timestamp t1 = Input_TimeStamp();
		printf("Enter the timestamp for ending of time period:\n");
		Timestamp t2 = Input_TimeStamp();
		Transactions_in_timeperiod(t1, t2, first_t);
			
		printf("\n");
		//Calculate total revenue by seller
		printf("Enter the Seller ID for whom total revenue is to be calculated: \n");
		int S_ID_rev;
		scanf("%d",&S_ID_rev);
		float total_revenue;
		total_revenue=TotalRevenue(S_ID_rev,first_s);
		printf("The total revenue of the Seller ID %d : %.2f.\n",S_ID_rev,total_revenue);
			
		printf("\n");
		//Sort transactions in decreasing order of energy traded
		first_t=MergeSort_dec_t(first_t);
		Set_Last_t(first_t,&last_t);
		printf("The transaction with the Highest Energy Amount is:\n");
		Transaction *t_ptr=first_t;
		//Considering the case that there may be multiple transactions with highest energy amount.
		while(t_ptr->EnergyAmount == first_t->EnergyAmount)
		{
			printf(" ~Transaction ID: %d\n",t_ptr->TransactionID);
			printf(" ~Energy Amount: %.2f\n",t_ptr->EnergyAmount);
			printf("\n");
			t_ptr=t_ptr->next;
	    }
	
		//print the sorted transaction list
		printf("Displaying the sorted list of Transactions: \n");
		Print_dec_Sorted_Transaction(first_t);
			
		printf("\n");	
		//Sort list of Buyers based on energy bought (increasing)
		first_b=MergeSort_inc_b(first_b);
		Set_Last_b(first_b,&last_b);
		//print the sorted transaction list
		printf("Displaying the sorted list of Buyers: \n");
		Print_inc_Sorted_Buyer(first_b);
			
		printf("\n");
		//Sort Seller-Buyer-Pair list in decreasing order of number of trasactions.
		first_sb=MergeSort_dec_sb(first_sb);
		Set_Last_sb(first_sb,&last_sb);
		//print the sorted transaction list
		printf("Displaying the sorted list of Seller-Buyer Pairs: \n");
		Print_dec_Sorted_SellerBuyer(first_sb);
			
		printf("\n");
		//Update details of the existing transaction.
		printf("Enter the Transaction ID whose details need to be updated: \n");
		int T_ID_update;
		scanf("%d",&T_ID_update);
		UpdateTransaction (file1,file2,T_ID_update,first_t,first_s,first_b);
		
		rewind(file1);
		t_ptr=first_t;
		fprintf(file1,"srn TID BID SID Energy  Price DD MM YYYY hr mn sc\n");
	
		while(t_ptr!=NULL)
		{
	    	fprintf(file1,"%03d %03d %03d %03d %.2f %.2f %02d %02d %02d %02d %02d %02d\n",t_ptr->sr_no, t_ptr->TransactionID, t_ptr->BuyerID, t_ptr->SellerID, t_ptr->EnergyAmount, t_ptr->price_pkWh, t_ptr->timestamp.date.day, t_ptr->timestamp.date.month, t_ptr->timestamp.date.year, t_ptr->timestamp.time.hours, t_ptr->timestamp.time.min, t_ptr->timestamp.time.sec);
			t_ptr=t_ptr->next;	
		}
		fflush(file1);
		//printf("Transaction file modified\n");//
		
		rewind(file2);
		//printf("Modifying seller file\n");//
		fprintf(file2,"Srn SID pbelow pabove\n");
		Seller *s_ptr=first_s;
		while(s_ptr!=NULL)
		{
	    	fprintf(file2,"%03d %03d %.2f %.2f\n",s_ptr->Sr_n,s_ptr->SellerID,s_ptr->p_below_300kWh,s_ptr->p_above_300kWh);
	    	s_ptr=s_ptr->next;
		}
		fflush(file2);
		//printf("Seller file modified\n");//				
		
	}
		
    fclose(file1);
    fclose(file2);
	return 0;	
}
