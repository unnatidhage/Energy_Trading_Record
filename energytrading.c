#include "energytrading.h"

//Creating Transaction list.

Transaction* Create_Transaction_Node (int sr_no, int T_ID, int B_ID, int S_ID, float E_Amount, float price_pkWh, Timestamp timestamp)
{
	Transaction* tptr;
	tptr=(Transaction*)malloc(sizeof(Transaction));
	if(tptr != NULL)
	{
		tptr->sr_no = sr_no;
		tptr->TransactionID = T_ID;
		tptr->BuyerID = B_ID;
		tptr->SellerID = S_ID;
		tptr->EnergyAmount = E_Amount;
		tptr->price_pkWh = price_pkWh;
		tptr->timestamp = timestamp;
		tptr->next = NULL;	
	}	
	return tptr;
}

void Create_Transaction_List(FILE* file , Transaction** first_tptr, Transaction** last_tptr)
{
	Transaction *t_node;
	//Skipping the first Header line.
	char buffer[256];
	fgets(buffer,sizeof(buffer),file);
	
	int sr_no;
	int T_ID;
	int B_ID;
	int S_ID;
	float E_Amount;
	float price_pkWh;
	int dd,mm,yyyy,hr,min,sec;
	//Reading the data
	while (fscanf(file,"%d %d %d %d %f %f %d %d %d %d %d %d",&sr_no,&T_ID,&B_ID,&S_ID,&E_Amount,&price_pkWh,&dd,&mm,&yyyy,&hr,&min,&sec) == 12)
	{
        Timestamp timestamp;
        timestamp.date.day=dd;
        timestamp.date.month=mm;
        timestamp.date.year=yyyy;
        timestamp.time.hours=hr;
        timestamp.time.min=min;
        timestamp.time.sec=sec;
		t_node=Create_Transaction_Node (sr_no,T_ID,B_ID,S_ID,E_Amount,price_pkWh,timestamp);
		
		if(t_node==NULL)
		{
			printf("Memory Allocation FAILED at %d th entry.\n",sr_no);
		}
		else
		{
			if(*first_tptr==NULL)
			{
				*first_tptr = *last_tptr = t_node;
			}
			else
			{
				(*last_tptr)->next=t_node;
				(*last_tptr)=t_node;
			}
		}
	}
}

//Creating Buyer List.

void Create_Buyer_List(Transaction *first_t, Buyer **first_bptr, Buyer **last_bptr)
{
    if(first_t!=NULL)
    {
    	int flag=0;
    	Transaction* t_ptr=first_t;
    	while(t_ptr!=NULL && flag==0)
    	{
    		Transaction_ptr * ptr=(Transaction_ptr*)malloc(sizeof(Transaction_ptr));
    		if(ptr != NULL)
    		{
    			ptr->tptr=t_ptr;
    		    ptr->next=NULL;
  		    
    		    int found=0;
    		    Buyer* b_ptr=*first_bptr;
    		    while(b_ptr != NULL && found==0)
    		    {
			        if(b_ptr->BuyerID == t_ptr->BuyerID)
				    {
		                ptr->next = b_ptr->t_lptr;
		                b_ptr->t_lptr=ptr;
		                b_ptr->num_Trans= (b_ptr->num_Trans)+ 1;
		                b_ptr->Total_Energy= (b_ptr->Total_Energy) + (t_ptr->EnergyAmount);
		                found=1;
				    }
				    b_ptr=b_ptr->next;	
			    }
			    if(found==0)
			    {
				    Buyer *b_node=(Buyer*)malloc(sizeof(Buyer));
				    if(b_node!=NULL)
				    {
					    b_node->BuyerID=t_ptr->BuyerID;
					    b_node->t_lptr=ptr;
					    b_node->num_Trans=1;
					    b_node->Total_Energy=t_ptr->EnergyAmount;
					    b_node->next=NULL;
					
					    if(*first_bptr==NULL)
					    {
						    *first_bptr=*last_bptr=b_node;
					    }
					    else
					    {
						    (*last_bptr)->next=b_node;
						    *last_bptr=b_node;
					    }
				    }
				    else
				    {
					    printf("Memory Allocation failed for Buyer node at %d th entry.\n", t_ptr->sr_no);
				    }
                }
            }
			else
			{
				printf("Transaction list cannot be created for Buyer because of problem in memory allocation.\n");
				flag=1;
			}
			t_ptr=t_ptr->next;
		}		
    }
	else
	{
		printf("Transaction-info list is empty. Buyer list cannot be created.\n");
	}
}

//Creating Seller list

void Create_Seller_List( FILE* file, Transaction* first_t, Seller** first_sptr, Seller** last_sptr)
{
	Seller *s_node;
	//Skipping the first Header line.
	char buffer[256];
	fgets(buffer,sizeof(buffer),file);
	
	int sr_n;
	int S_ID;
	float p_b_300kWh;
	float p_a_300kWh;
	//Reading the data.
	while(fscanf(file,"%d %d %f %f",&sr_n,&S_ID,&p_b_300kWh,&p_a_300kWh) == 4)
	{
		s_node = (Seller*)malloc(sizeof(Seller));
	    if(s_node==NULL)
	    {
		    printf("Error in memory allocation for seller node.\n");
	    }
	    else
	    {
	    	s_node->Sr_n=sr_n;
			s_node->SellerID=S_ID;
			s_node->p_below_300kWh=p_b_300kWh;
			s_node->p_above_300kWh=p_a_300kWh;
			s_node->t_lptr=NULL;
			s_node->Regular_Buyers=NULL;
			s_node->next=NULL;
			
			if(*first_sptr==NULL)
			{
				*first_sptr=*last_sptr=s_node;
			}
			else
			{
				(*last_sptr)->next=s_node;
				*last_sptr=s_node;
			}
		}	
	}
}

void Add_Transaction_List_Seller(Seller *first_s, Transaction *first_t )
{
	Seller *s_ptr = first_s;
	Transaction *t_ptr=first_t;
	int flag=0;
	    
	if(first_t!=NULL)
	{	
		while(s_ptr!=NULL && flag==0)
	    {
	    	t_ptr=first_t;
		    while(t_ptr !=NULL && flag==0)
		    {
			    if(t_ptr->SellerID == s_ptr->SellerID)
			    {
				    Transaction_ptr *ptr=(Transaction_ptr*)malloc(sizeof(Transaction_ptr));
    		        if(ptr != NULL)
    		        {
    			        ptr->tptr=t_ptr;
    			        ptr->next=NULL;
    		            if(s_ptr->t_lptr==NULL)
    		            {
    		        	    s_ptr->t_lptr=ptr;
					    }
					    else
					    {
						    ptr->next=s_ptr->t_lptr;
						    s_ptr->t_lptr=ptr;	
					    }
			        }
			        else
				    {
					    flag=1;
				        printf("Transaction list cannot be created for Seller because of problem in memory allocation.\n");
		     	    }
		     	
			    }
			    t_ptr=t_ptr->next;
		    }
		    s_ptr=s_ptr->next;
	    }
	}
	else
	{
		printf("Transaction-info list is empty. Seller list cannot be created.");
	}
}

void Add_Regular_Buyer_List(Seller_Buyer_Pair *first_sb,Seller *first_s)
{
	Seller *s_ptr= first_s;
	Seller_Buyer_Pair *sb_ptr;
	
	while(s_ptr != NULL)
	{
		sb_ptr=first_sb;
		while(sb_ptr != NULL)
		{
			if(sb_ptr->SellerID == s_ptr->SellerID)
			{
				if(sb_ptr->num_Trans > 5)
				{
					Buyer_ptr* ptr= (Buyer_ptr*)malloc(sizeof(Buyer_ptr));
					if(ptr != NULL)
					{
						ptr->BuyerID=sb_ptr->BuyerID;
					    ptr->next=NULL;
						if(s_ptr->Regular_Buyers == NULL)
						{
							s_ptr->Regular_Buyers=ptr;
						}
						else
						{
							ptr->next=s_ptr->Regular_Buyers;
							s_ptr->Regular_Buyers=ptr;							
						}
					}
				}
			}
			sb_ptr=sb_ptr->next;
		}
		s_ptr=s_ptr->next;
	}	
}

//Create Seller-Buyer Pair List

void Create_Seller_Buyer_Pair_List(Transaction *first_t, Seller_Buyer_Pair **first_sbptr, Seller_Buyer_Pair **last_sbptr)
{
	if(first_t!=NULL)
    {
    	int flag=0;
    	Transaction* t_ptr=first_t;
    	while(t_ptr!=NULL && flag==0)
    	{
    		Transaction_ptr *ptr=(Transaction_ptr*)malloc(sizeof(Transaction_ptr));
    		if(ptr != NULL)
    		{
    			ptr->tptr=t_ptr;
    		    ptr->next=NULL;
    		    
    		    Seller_Buyer_Pair *sb_ptr=*first_sbptr;
    		    int found=0;
    	    	while(sb_ptr != NULL && found==0)
    		    {
			        if(sb_ptr->BuyerID == t_ptr->BuyerID && sb_ptr->SellerID == t_ptr->SellerID)
				    {
		                ptr->next= sb_ptr->t_lptr;
		                sb_ptr->t_lptr=ptr;
		            	sb_ptr->num_Trans= (sb_ptr->num_Trans)+ 1;
		            	found=1;
					}
					sb_ptr=sb_ptr->next;	
				}
				if(found==0)
				{
					Seller_Buyer_Pair *sb_node=(Seller_Buyer_Pair*)malloc(sizeof(Seller_Buyer_Pair));
					if(sb_node!=NULL)
					{
						sb_node->BuyerID=t_ptr->BuyerID;
						sb_node->t_lptr=ptr;
						sb_node->num_Trans=1;
						sb_node->SellerID=t_ptr->SellerID;
						sb_node->next=NULL;
					
						if(*first_sbptr==NULL)
						{
							*first_sbptr=(*last_sbptr)=sb_node;
						}
						else
						{
							(*last_sbptr)->next=sb_node;
							*last_sbptr=sb_node;
						}
					}
					else
					{
						printf("Memory Allocation failed for Seler-Buyer pair node.\n");
						flag=1;
					}
				}
			}
			else
			{
				printf("Transaction list cannot be created for Seller-Buyer Pair because of problem in memory allocation.\n");
				flag=1;
			}   	    
			t_ptr=t_ptr->next;
		}		
    }
	else
	{
		printf("Transaction-info list is empty. Seller-Buyer Pair list cannot be created.\n");
	}
}

//  Taking input of time

Timestamp Input_TimeStamp()
{
	int dd , mm,  yyyy,  hr,  min, sec;
	printf("\nEnter date in the form of DD MM YYYY: \n");
	scanf("%d %d %d",&dd , &mm, &yyyy);
	printf("Enter the time in 24 hours clock in the form hr min sec:\n");
	scanf("%d %d %d", &hr, &min, &sec);
	Timestamp t;
	t.date.day=dd;
	t.date.month=mm;
	t.date.year=yyyy;
	t.time.hours=hr;
	t.time.min=min;
	t.time.sec=sec;
	return t;
}

//add a new transaction to the system

status_code Add_New_Transaction(FILE *file1, FILE *file2 ,Transaction **last_tptr,Buyer *first_b,Buyer **last_bptr, Seller *first_s, Seller **last_sptr,int T_ID, int B_ID, int S_ID, float E_Amount, Timestamp timestamp)
{
	status_code sc=SUCCESS;
	Seller *s_ptr=first_s;
	
	Transaction *t=(Transaction*)malloc(sizeof(Transaction));
	if(t==NULL)
	{
		sc= FAILURE;
	}
	else
	{
		t->sr_no=((*last_tptr)->sr_no)+1;
		t->TransactionID=T_ID;
		t->BuyerID=B_ID;
		t->SellerID=S_ID;
		t->EnergyAmount=E_Amount;
		t->timestamp=timestamp;
		
		int found=0;
		
		while((s_ptr != NULL) && (found==0))
		{
			if(s_ptr->SellerID == S_ID)
			{
				if(E_Amount<300)
				{
					t->price_pkWh=s_ptr->p_below_300kWh;
				}
				else
				{
					t->price_pkWh=s_ptr->p_above_300kWh;
				}
				found=1;
			}
			if(found==0)
			{
			    s_ptr=s_ptr->next;
		    }
	    }
	    t->next=NULL;
	    (*last_tptr)->next=t;
	    *last_tptr=t;
		    
	    if(found==0)
	    {
	    	Seller *ptr= (Seller*)malloc(sizeof(Seller));
	    	if(ptr == NULL)
	    	{
	    		printf("Seller node cannot be created for the new seller input due to problem in memory allocation.\n");
			}
			else
			{
				ptr->SellerID=S_ID;
				ptr->Regular_Buyers=NULL;
				
				Transaction_ptr *ptr2=(Transaction_ptr*)malloc(sizeof(Transaction_ptr));
				if(ptr2 != NULL)
				{
					ptr2->tptr=t;
					ptr2->next=NULL;
				}
				else
				{
					printf("Transaction list cannot be created for the new Seller.\n");
				}
				ptr->t_lptr=ptr2;
				
				printf("Enter the price per kWh for the new seller below 300kWh:\n");
				scanf("%f",&ptr->p_below_300kWh);
				printf("Enter the price per kWh for the new seller above 300kWh:\n");
				scanf("%f",&ptr->p_above_300kWh);
				
				if(E_Amount < 300)
				{
					t->price_pkWh=ptr->p_below_300kWh;
				}
				else
				{
					t->price_pkWh=ptr->p_above_300kWh;
				}
	
				ptr->next=NULL;
				ptr->Sr_n=((*last_sptr)->Sr_n +1);
//				fseek(file2,0,SEEK_END);
//				fprintf(file2," %d %d %.2f %.2f\n",ptr->Sr_n,ptr->SellerID,ptr->p_below_300kWh,ptr->p_above_300kWh);
//				fflush(file2);
//				
				(*last_sptr)->next=ptr;
				*last_sptr=ptr;	
				s_ptr=ptr;
		    }
		}
		Transaction_ptr* temp_ptr=(Transaction_ptr*)malloc(sizeof(Transaction_ptr));
		if(temp_ptr != NULL)
		{
			temp_ptr->tptr=(*last_tptr);
			if(s_ptr->t_lptr != NULL) //updating transaction list for the seller
			{
		    	temp_ptr->next=s_ptr->t_lptr; 
		    	s_ptr->t_lptr=temp_ptr;
	    	}
	    	else
	    	{
	    		temp_ptr->next=NULL;
	    		s_ptr->t_lptr=temp_ptr;
			}
	    }
	    else
		{
			printf("Transaction list could not be updated for the seller in newly added transaction due to failure in memory allocation!!\n");
		}
		
		Buyer *b_ptr=first_b;
		while(b_ptr!=NULL && b_ptr->BuyerID != B_ID)
		{
			b_ptr=b_ptr->next;
		}

		if(b_ptr== NULL)
		{
			Buyer *temp_b=(Buyer*)malloc(sizeof(Buyer));
			if(temp_b != NULL)
			{
				temp_b->BuyerID=B_ID;
				temp_b->num_Trans=1;
				temp_b->Total_Energy=E_Amount;
				temp_b->t_lptr=NULL;
				temp_b->next=NULL;
				(*last_bptr)->next=temp_b;
				*last_bptr=temp_b;
				b_ptr=temp_b;
			}
			else
			{
				printf("Buyer list could not be updated for the new buyer added in the new transcations due to problem in memory allocation.\n");
			}	
		}
		
		Transaction_ptr* temp_ptr2=(Transaction_ptr*)malloc(sizeof(Transaction_ptr));
		if(temp_ptr2 != NULL)
		{
			temp_ptr2->tptr=(*last_tptr);
			if(b_ptr->t_lptr != NULL) //updating transaction list for the seller
			{
		    	temp_ptr2->next=b_ptr->t_lptr; 
	    	}
	    	else
	    	{
	    		temp_ptr2->next=NULL;
			}
			b_ptr->t_lptr=temp_ptr2;
	    }
	    else
		{
			printf("Transaction list could not be updated for the buyer in newly added transaction due to failure in memory allocation!!\n");
		}
		
		//Move the file pointer to the end to append new content
//		fseek(file1,0,SEEK_END);
//		fprintf(file1," %d %d %d %d %.2f %.2f %d %d %d %d %d %d\n",t->sr_no, T_ID, B_ID, S_ID, E_Amount, t->price_pkWh, timestamp.date.day, timestamp.date.month, timestamp.date.year, timestamp.time.hours, timestamp.time.min, timestamp.time.sec);
//	    fflush(file1);
	}
	return sc;	
}

//Display All Transaction Details

void Display_Transactions(Transaction *first_t)
{
	printf("All the transactions are displayed below:\n");
	Transaction *t_ptr=first_t;
	while(t_ptr!=NULL)
    {
       printf("%d) ",t_ptr->sr_no);
       printf("Transaction ID:%d\n",t_ptr->TransactionID); 
       printf("  ~Buyer ID:%d\n",t_ptr->BuyerID);
       printf("  ~Seller ID:%d\n",t_ptr->SellerID);
       printf("  ~Energy Amount (in kWh):%f\n",t_ptr->EnergyAmount);
       printf("  ~Price per kWh:%f\n",t_ptr->price_pkWh);
       printf("  ~Date: %d/%d/%d\n",t_ptr->timestamp.date.day,t_ptr->timestamp.date.month,t_ptr->timestamp.date.year);
       printf("  ~Time: %d:%d:%d\n",t_ptr->timestamp.time.hours,t_ptr->timestamp.time.min,t_ptr->timestamp.time.sec);
       printf("\n"); 
	   
	   t_ptr=t_ptr->next;     
    }
}

//Display Transaction list of Seller
void Display_List_Transaction_Seller(Seller *first_s)
{
	Seller *s_ptr = first_s;
	printf("The lists of transcations for all the sellers are displayed below:\n");
	while(s_ptr != NULL)
	{
		printf("Transactions for Seller-%d: \n",s_ptr->SellerID);
		Transaction_ptr *ptr =s_ptr->t_lptr;
		while(ptr != NULL)
		{
		    printf("  ~ %d\n",((ptr->tptr)->TransactionID));
		    ptr=ptr->next;
	    }
	    s_ptr=s_ptr->next;
	}	
}

//Display Transaction list of Buyer
void Display_List_Transaction_Buyer(Buyer *first_b)
{
	Buyer *b_ptr = first_b;
	printf("The lists of transcations for all the buyers are displayed below:\n");
	while(b_ptr != NULL)
	{
		printf("Transactions for Buyer-%d: \n",b_ptr->BuyerID);
		Transaction_ptr *ptr =b_ptr->t_lptr;
		while(ptr != NULL)
		{
		    printf("  ~ %d\n",(ptr->tptr)->TransactionID);
		    ptr=ptr->next;
	    }
	    b_ptr=b_ptr->next;
	}	
}

//Total Revenue By Seller
float TotalRevenue (int S_ID, Seller *first_s)
{
	Seller *s_ptr = first_s;
	int found=0;
	float total_revenue=0;
	while(s_ptr != NULL && found==0)
	{
		if(s_ptr->SellerID == S_ID)
		{
			found=1;
			Transaction_ptr *ptr =s_ptr->t_lptr;
			while(ptr != NULL)
			{
				total_revenue = total_revenue + (((ptr->tptr)->EnergyAmount)*((ptr->tptr)->price_pkWh));
				ptr=ptr->next;	
			}					
		}
		s_ptr=s_ptr->next;
	}
	return total_revenue;
}

//Update details of existing transaction

void Update_Energy(Transaction *t_ptr , Buyer *b_ptr, Seller *s_ptr, float energy)
{
	b_ptr->Total_Energy = b_ptr->Total_Energy - t_ptr->EnergyAmount + energy;
	t_ptr->EnergyAmount = energy;
	if(energy <= 300)
	{
		t_ptr->price_pkWh=s_ptr->p_below_300kWh;
	}
	else
	{
		t_ptr->price_pkWh=s_ptr->p_above_300kWh;
	}
	printf("Energy updated successfully!\n");	
}

void Update_Price(Transaction *t_ptr, Seller *s_ptr,float price)
{
	t_ptr->price_pkWh=price;
//	if(t_ptr->EnergyAmount <= 300)
//	{
//		s_ptr->p_below_300kWh=price;
//	}
//	else
//	{
//		s_ptr->p_above_300kWh=price;
//	}
	printf("Price updated successfully!\n");
}
void Update_Timestamp(Transaction *t_ptr, Timestamp t)
{
	t_ptr->timestamp=t;
	printf("Timestamp updated successfully!\n");
}


void UpdateTransaction (FILE *file1, FILE *file2, int T_ID, Transaction *first_t, Seller *first_s, Buyer *first_b)
{
	Transaction *t_ptr = first_t;
	Buyer *b_ptr = first_b;
	Seller *s_ptr = first_s;
	int flag=0;	
	while(t_ptr != NULL && flag==0)
	{
		if(t_ptr->TransactionID == T_ID)
		{
			flag=1;
			while(b_ptr->BuyerID != t_ptr->BuyerID) //since this is an existing transaction so buyer id will be there in the buyer list.
			{
				b_ptr = b_ptr->next;
			}
			
			while(s_ptr->SellerID != t_ptr->SellerID)
			{
				s_ptr=s_ptr->next;
			}
			
			printf("Enter the number corresponding to the parameter you want to change,\n");
			printf("Amount of Energy :- 1\n");
			printf("Price per kWh :- 2\n");
			printf("Timestamp :- 3\n");
			
			int idx;
			scanf("%d",&idx);
			
			switch(idx)
			{
				case 1 :
					printf("Enter the modified Energy Amount: \n");
					float energy;
					scanf("%f",&energy);
					Update_Energy(t_ptr,b_ptr,s_ptr,energy);
					break;
				
				case 2 :
					printf("Enter the modified Price per kWh: \n");
					float price;
					scanf("%f",&price);
					Update_Price(t_ptr,s_ptr,price);
					
//					fseek(file2,0,SEEK_SET);
//					int j=0;
//					while(j <= s_ptr->Sr_n)
//					{
//						char buffer1[256];
//	            	    fgets(buffer1,sizeof(buffer1),file2);
//	            	    j++;
//					}
//					fprintf(file2," %d %d %.2f %.2f",s_ptr->Sr_n,s_ptr->SellerID,s_ptr->p_below_300kWh,s_ptr->p_above_300kWh);
//					fflush(file2);
					break;
				
				case 3 :
					printf("Enter the modified timestamp as guided below: \n");
					Timestamp t = Input_TimeStamp();
					Update_Timestamp(t_ptr, t);
					break;
				
				default :
					printf("you entered an invalid number!\n");
					break;

			}
				
//			fseek(file1,0,SEEK_SET);
//			int i=0;
//			while(i <= t_ptr->sr_no)
//			{
//				char buffer2[256];
//	            fgets(buffer2,sizeof(buffer2),file1);
//				i++;
//	        }
//			fprintf(file1," %d %d %d %d %.2f %.2f %d %d %d %d %d %d",t_ptr->sr_no, t_ptr->TransactionID, t_ptr->BuyerID, t_ptr->SellerID, t_ptr->EnergyAmount, t_ptr->price_pkWh, t_ptr->timestamp.date.day, t_ptr->timestamp.date.month, t_ptr->timestamp.date.year, t_ptr->timestamp.time.hours, t_ptr->timestamp.time.min, t_ptr->timestamp.time.sec);
//			fflush(file1);
		}
		t_ptr=t_ptr->next;
	}
	if(flag==0)
	{
		printf("No transaction with the entered Transaction ID could be found. Updation Failed !!\n");
	}
	else
	{
		printf("The details are updated successfully!\n");
	}
}

//Divide Transactions in two halfs

Transaction* Divide_t(Transaction *first_t)
{
	Transaction *t_ptr, *slow, *fast;
	slow=first_t;
	fast=first_t->next->next;
	
	while(fast != NULL)
	{
		slow= slow->next;
		fast= fast->next;
		if(fast!=NULL)
		{
			fast=fast->next;
		}
	}
	t_ptr=slow->next;
	slow->next=NULL;
	return t_ptr;
}


//Divide buyer list in two halfs
Buyer* Divide_b(Buyer *first_b)
{
	Buyer *b_ptr, *slow, *fast;
	slow=first_b;
	fast=first_b->next->next;
	
	while(fast != NULL)
	{
		slow= slow->next;
		fast= fast->next;
		if(fast!=NULL)
		{
			fast=fast->next;
		}
	}
	b_ptr=slow->next;
	slow->next=NULL;
	return b_ptr;
}

//Divide Seller-Buyer pair list in two halfs
Seller_Buyer_Pair* Divide_sb(Seller_Buyer_Pair *first_sb)
{
	Seller_Buyer_Pair *sb_ptr, *slow, *fast;
	slow=first_sb;
	fast=first_sb->next->next;
	
	while(fast != NULL)
	{
		slow= slow->next;
		fast= fast->next;
		if(fast!=NULL)
		{
			fast=fast->next;
		}
	}
	sb_ptr=slow->next;
	slow->next=NULL;
	return sb_ptr;
}

//Merging two decreasingly sorted-halfs in a single sorted transaction list.

Transaction* Merge_t(Transaction *list1, Transaction *list2)
{
	Transaction *ptr1 = list1;
	Transaction *ptr2 = list2;
	Transaction *tail, *result;
	if(ptr1->EnergyAmount > ptr2->EnergyAmount)
	{
	 	result=ptr1;
	 	tail=ptr1;
	 	ptr1=ptr1->next;
	}
	else
	{
		result=ptr2;
		tail=ptr2;
	    ptr2=ptr2->next;
	}
	
	while((ptr1 != NULL) && (ptr2 != NULL))
	{
		if(ptr1->EnergyAmount > ptr2->EnergyAmount)
		{
			tail->next=ptr1;
			tail=tail->next;
			ptr1=ptr1->next;
		}
		else
		{
			tail->next=ptr2;
			tail=tail->next;
			ptr2=ptr2->next;
		}
	}
	if(ptr1 != NULL)
	{
		tail->next=ptr1;
	}
	else
	{
		tail->next=ptr2;
	}
	return result;
}

//Merging two increasingly sorted halfs of buyer list in a single sorted list.

Buyer* Merge_b(Buyer *list1, Buyer *list2)
{
	Buyer *ptr1 = list1;
	Buyer *ptr2 = list2;
	Buyer *tail, *result;
	if(ptr1->Total_Energy < ptr2->Total_Energy)
	{
	 	result=ptr1;
	 	tail=ptr1;
	 	ptr1=ptr1->next;
	}
	else
	{
		result=ptr2;
		tail=ptr2;
	    ptr2=ptr2->next;
	}
	
	while((ptr1 != NULL) && (ptr2 != NULL))
	{
		if(ptr1->Total_Energy < ptr2->Total_Energy)
		{
			tail->next=ptr1;
			tail=tail->next;
			ptr1=ptr1->next;
		}
		else
		{
			tail->next=ptr2;
			tail=tail->next;
			ptr2=ptr2->next;
		}
	}
	if(ptr1 != NULL)
	{
		tail->next=ptr1;
	}
	else
	{
		tail->next=ptr2;
	}
	return result;
}

//Merging two decreasingly sorted halfs of buyer list in a single sorted list.

Seller_Buyer_Pair* Merge_sb(Seller_Buyer_Pair *list1, Seller_Buyer_Pair *list2)
{
	Seller_Buyer_Pair *ptr1 = list1;
	Seller_Buyer_Pair *ptr2 = list2;
	Seller_Buyer_Pair *tail, *result;
	if(ptr1->num_Trans > ptr2->num_Trans)
	{
	 	result=ptr1;
	 	tail=ptr1;
	 	ptr1=ptr1->next;
	}
	else
	{
		result=ptr2;
		tail=ptr2;
	    ptr2=ptr2->next;
	}
	
	while((ptr1 != NULL) && (ptr2 != NULL))
	{
		if(ptr1->num_Trans > ptr2->num_Trans)
		{
			tail->next=ptr1;
			tail=tail->next;
			ptr1=ptr1->next;
		}
		else
		{
			tail->next=ptr2;
			tail=tail->next;
			ptr2=ptr2->next;
		}
	}
	if(ptr1 != NULL)
	{
		tail->next=ptr1;
	}
	else
	{
		tail->next=ptr2;
	}
	return result;
}

//Mergesort_decreasing_transactions

Transaction* MergeSort_dec_t(Transaction *first_t)
{
	Transaction *t_ptr, *list_ptr;
	list_ptr=first_t;
	
	if(first_t != NULL && first_t->next != NULL)
	{
		t_ptr = Divide_t(list_ptr);
		list_ptr = MergeSort_dec_t(list_ptr);
		t_ptr = MergeSort_dec_t(t_ptr);
		first_t=Merge_t(list_ptr,t_ptr);
	}	
	return first_t;
}
//Setting last_t after sorting
void Set_Last_t(Transaction *first_t,Transaction **last_tptr)
{
	Transaction *t_ptr=first_t;
	while(t_ptr->next!=NULL)
	{
		t_ptr=t_ptr->next;
	}
	*last_tptr=t_ptr;
}
//Printing decreasingly sorted list of transaction nodes.
void Print_dec_Sorted_Transaction(Transaction *first_t)
{
	Transaction *t_ptr;
	t_ptr=first_t;
	while(t_ptr!=NULL)
	{
		printf("Transcation ID: %d --Energy Amount: %f\n",t_ptr->TransactionID,t_ptr->EnergyAmount);
		t_ptr=t_ptr->next;
	}
}

//Mergesort_increasing_buyers
Buyer* MergeSort_inc_b(Buyer *first_b)
{
	Buyer *b_ptr, *list_ptr;
	list_ptr=first_b;
	
	if(first_b != NULL && first_b->next != NULL)
	{
		b_ptr = Divide_b(list_ptr);
		list_ptr = MergeSort_inc_b(list_ptr);
		b_ptr = MergeSort_inc_b(b_ptr);
		first_b=Merge_b(list_ptr,b_ptr);
	}
	return first_b;
}
//Setting last_b after sorting
void Set_Last_b(Buyer *first_b,Buyer **last_bptr)
{
	Buyer *b_ptr=first_b;
	while(b_ptr->next!=NULL)
	{
		b_ptr=b_ptr->next;
	}
	*last_bptr=b_ptr;
}
//Printing increasingly sorted list of buyer nodes.
void Print_inc_Sorted_Buyer(Buyer *first_b)
{
	Buyer *b_ptr;
	b_ptr=first_b;
	while(b_ptr!=NULL)
	{
		printf("Buyer ID: %d --Energy Amount: %f\n",b_ptr->BuyerID,b_ptr->Total_Energy);
		b_ptr=b_ptr->next;
	}
}
//MergeSort_decreasing_seller-buyer-pairs

Seller_Buyer_Pair* MergeSort_dec_sb(Seller_Buyer_Pair *first_sb)
{
	Seller_Buyer_Pair *sb_ptr, *list_ptr;
	list_ptr=first_sb;
	
	if(first_sb != NULL && first_sb->next != NULL)
	{
		sb_ptr = Divide_sb(list_ptr);
		list_ptr = MergeSort_dec_sb(list_ptr);
		sb_ptr = MergeSort_dec_sb(sb_ptr);
		first_sb=Merge_sb(list_ptr,sb_ptr);
	}
	return first_sb;
}
//Setting last_sb after sorting
void Set_Last_sb(Seller_Buyer_Pair *first_sb, Seller_Buyer_Pair **last_sbptr)
{
	Seller_Buyer_Pair *sb_ptr=first_sb;
	while(sb_ptr->next!=NULL)
	{
		sb_ptr=sb_ptr->next;
	}
	*last_sbptr=sb_ptr;
}
//Printing decreasingly sorted list of transaction nodes.
void Print_dec_Sorted_SellerBuyer(Seller_Buyer_Pair *first_sb)
{
	Seller_Buyer_Pair *sb_ptr;
	sb_ptr=first_sb;
	while(sb_ptr!=NULL)
	{
		printf("Seller ID: %d  ~  Buyer ID: %d  --Number of transactions: %d\n",sb_ptr->SellerID,sb_ptr->BuyerID,sb_ptr->num_Trans);
		sb_ptr=sb_ptr->next;
	}
}

//Function to check if a year is leap year
int isLeapYear(int year)
{
	int ret_value=0;
	if((year%4 == 0 && year % 100 != 0) || year % 400 == 0)
	{
		ret_value=1;
	}
	return ret_value;
}

//Funtion to calculate total days 
long calculateDays(Date d)
{
	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	long totalDays = 0;
	
	//Adding days from previos year
	for(int i=1; i<d.year; i++)
	{
		if(isLeapYear(i)==1)
		{
			totalDays=totalDays+366;
		}
		else
		{
			totalDays=totalDays+365;
		}
	}
	//Adding days from previous month
	for(int j=0; j< d.month-1 ; j++)
	{
		if(j==1 && isLeapYear(d.year))
		{
			totalDays=totalDays+29;
		}
		else
		{
			totalDays=totalDays+daysInMonth[j];
		}
	}	
	totalDays=totalDays+d.day -1; //Adding complete days from current month
    return totalDays;		
}
//Calculate seconds for a timestamp
long Calculate_sec(Timestamp t)
{
	long ret_val = t.time.sec + ((t.time.min)*60) + ((t.time.hours)*3600); //timestamp->time included.
	ret_val = ret_val + (calculateDays(t.date))*24*3600;
	
	return ret_val;	
}

//Trnsactions in a given time period.
void Transactions_in_timeperiod(Timestamp t1, Timestamp t2, Transaction *first_t)
{
	Transaction *t_ptr=first_t;
	long sec_t1=Calculate_sec(t1);
	long sec_t2=Calculate_sec(t2);
	printf("The transactions taking place between the given time period are listed below:\n");
	while(t_ptr != NULL)
	{
	    long sec_t=Calculate_sec(t_ptr->timestamp);
	    if(sec_t >= sec_t1 && sec_t <= sec_t2)
	    {
	    	printf(" -> %d :\n",t_ptr->TransactionID);
	    	printf("    ~Date: %d/%d/%d\n",t_ptr->timestamp.date.day,t_ptr->timestamp.date.month,t_ptr->timestamp.date.year);
            printf("    ~Time: %d:%d:%d\n",t_ptr->timestamp.time.hours,t_ptr->timestamp.time.min,t_ptr->timestamp.time.sec);
            printf("\n"); 
		}
		t_ptr=t_ptr->next;
	}	
}






