#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<limits.h>
#include<gsl/gsl_randist.h>




//Agent strategies parameters
#define UO 0	//Unsocial, Optimistic
#define UR 1	//Unsocial, Realistic
#define SO 2	//Social, Optimistic
#define SR 3	//Social, Realistic



//agent actions
#define NO_ACTION 	0
#define A_TRUE 		1
#define B_TRUE 		2
#define A_FALSE 	3
#define B_FALSE 	4
#define C_TRUE 		5
#define C_FALSE 	6


//Game environment: boolean variables A  and B and C
int environment = 0;


//LTL formulae trackers
//all must be called at each environment state update to determine satisfaction
int phi_satisfaction = 0;
int psi_satisfaction = 0;

int UO_satisfaction = 0;
int UR_satisfaction = 0;
int SO_satisfaction = 0;
int SR_satisfaction = 0;

void phi(int n_state)
{
	static int p_state = 0;
	static int state = 0;


	//check satisfaction
	if((p_state == 0) && (state == 1) && (n_state == 5))
		phi_satisfaction++;

	if((p_state == 2) && (state == 3) && (n_state == 5))
		phi_satisfaction++;

	if((p_state == 6) && (state == 7) && (n_state == 3))
		phi_satisfaction++;

	//update
	p_state = state;
	state = n_state;
}

void psi(int n_state)
{
	static int state = 0;

	//check satisfaction
	if((state == 2) && (n_state == 0))
		psi_satisfaction++;

	if((state == 3) && (n_state == 1))
		psi_satisfaction++;

	
	//update
	state = n_state;
}


void predicted_UO(int n_state)
{
	static int p_state = 0;
	static int state = 0;

	//check satisfaction
	if((p_state == 3) && (state == 5) && (n_state == 7))
		UO_satisfaction++;
	if((p_state == 5) && (state == 7) && (n_state == 3))
		UO_satisfaction++;
	if((p_state == 7) && (state == 3) && (n_state == 5))
		UO_satisfaction++;

	//update
	p_state = state;
	state = n_state;
}

void predicted_UR(int n_state)
{
	static int p_state = 0;
	static int state = 0;

	//check satisfaction
	if((p_state == 3) && (state == 5) && (n_state == 7))
		UR_satisfaction++;
	if((p_state == 5) && (state == 7) && (n_state == 3))
		UR_satisfaction++;
	if((p_state == 7) && (state == 3) && (n_state == 5))
		UR_satisfaction++;

	//update
	p_state = state;
	state = n_state;
}

void predicted_SO(int n_state)
{
	static int p_state0 = 0;
	static int p_state1 = 0;
	static int p_state2 = 0;
	static int p_state3 = 0;


	if((p_state3 == 1) && (n_state == 2))
		SO_satisfaction++;
	if((p_state3 == 2) && (n_state == 1))
		SO_satisfaction++;

	//update
	p_state0 = p_state1;
	p_state1 = p_state2;
	p_state2 = p_state3;
	p_state3 = n_state;
}

void predicted_SR(int n_state)
{
	static int state = 0;

	//check satisfaction
	if((state == 0) && (n_state == 3))
		SR_satisfaction++;
	if((state == 3) && (n_state == 0))
		SR_satisfaction++;

	//update
	state = n_state;
}


//Applies a formula of given length to the environment
void apply_formula(int formula[], int length);




#define BOTH 0
#define FIRST 1
#define SECOND 2


int c0_action(int environment, int strategy)
{
	switch(strategy)
	{
	case UO:{
			switch(environment)
			{
				case 0: return C_TRUE;
				case 1: return A_TRUE;
				case 2: return C_TRUE;
				case 3: return A_TRUE;
				case 4: return C_TRUE;
				case 5: return C_FALSE;
				case 6: return C_TRUE;
				case 7: return A_FALSE;
			}	
			} break;
	case UR:{
			switch(environment)
			{
				case 0: return C_TRUE;
				case 1: return A_TRUE;
				case 2: return C_TRUE;
				case 3: return A_TRUE;
				case 4: return C_TRUE;
				case 5: return C_FALSE;
				case 6: return C_TRUE;
				case 7: return A_FALSE;
			}			
			} break;
	case SO:{ 
			switch(environment)
			{
				case 0: return NO_ACTION;
				case 1: return C_FALSE;
				case 2: return C_TRUE;
				case 3: return A_TRUE;
				case 4: return A_FALSE;
				case 5: return C_FALSE;
				case 6: return A_FALSE;
				case 7: return C_FALSE;
			}	
			} break;
	case SR: 
			switch(environment)
			{
				case 0: return C_TRUE;
				case 1: return A_TRUE;
				case 2: return A_TRUE;
				case 3: return C_FALSE;
				case 4: return A_FALSE;
				case 5: return C_FALSE;
				case 6: return C_TRUE;
				case 7: return A_FALSE;
			}	
			break;
	}
	return 0;
}

int c1_action(int environment, int strategy)
{
	switch(strategy)
	{
	case UO:{
			switch(environment)
			{
				case 0: return B_TRUE;
				case 1: return B_TRUE;
				case 2: return B_FALSE;
				case 3: return B_FALSE;
				case 4: return B_TRUE;
				case 5: return B_TRUE;
				case 6: return B_TRUE;
				case 7: return B_TRUE;
			}	
			} break;
	case UR:{
			switch(environment)
			{
				case 0: return B_TRUE;
				case 1: return B_TRUE;
				case 2: return B_FALSE;
				case 3: return B_FALSE;
				case 4: return B_TRUE;
				case 5: return B_TRUE;
				case 6: return B_TRUE;
				case 7: return B_TRUE;
			}	
			} break;
	case SO:{ 
			switch(environment)
			{
				case 0: return B_TRUE;
				case 1: return B_TRUE;
				case 2: return B_FALSE;
				case 3: return NO_ACTION;
				case 4: return B_TRUE;
				case 5: return B_TRUE;
				case 6: return B_TRUE;
				case 7: return B_FALSE;
			}	
			} break;
	case SR: 
			switch(environment)
			{
				case 0: return B_TRUE;
				case 1: return B_TRUE;
				case 2: return B_FALSE;
				case 3: return B_FALSE;
				case 4: return B_TRUE;
				case 5: return B_TRUE;
				case 6: return B_TRUE;
				case 7: return B_TRUE;
			}	
			break;
	}
	return 0;
}


//allocates a random number generator "seeder", used for seed generation
gsl_rng *seeder;
//allocates a random number generator "rng", used for uniform generation
gsl_rng *rng;


//This is where probabilities kick in
int choice()
{

	unsigned int x = gsl_rng_get(rng);

	//int x = rand()%40;
	if((x % 4) == 2)
		return FIRST;
	if((x % 4) == 3)
		return SECOND;
	return BOTH;
}




int main(int argc, char *argv[])
{
	int strategy;

	if(argc != 2)
	{
		printf("Error: use \"<program name> <agent strategy>\"\n");
		return 0;
	}

	int total = 0;

	//allocates a random number generator "victory", used for uniform victory generation
	rng = gsl_rng_alloc (gsl_rng_taus);
	seeder = gsl_rng_alloc (gsl_rng_taus);

	//seeds the random number generator
	gsl_rng_set(seeder, time(NULL));


	strategy = atoi(argv[1]);

	
	

	int formula[2];

	int i;

	printf("Phi\tPsi\tPredicted\n");

	for(i=0;i<10000;i++)
	{
		gsl_rng_set(rng, gsl_rng_uniform_int(seeder, INT_MAX));
		


		//get agent strategy
		formula[0] = c0_action(environment,strategy);
		formula[1] = c1_action(environment,strategy);

		//determine which option
		int sequence = choice();

		//either apply both, then evaluate
		if(sequence == BOTH)
		{
			
			apply_formula(formula, 2);
			phi(environment);
			psi(environment);
			predicted_UO(environment);
			predicted_UR(environment);
			predicted_SO(environment);
			predicted_SR(environment);
		}
		else
		{
			//or apply one, then the other
			if(sequence == FIRST)
			{
		
				apply_formula(formula, 1);

				if(formula[0] != NO_ACTION)
				{
					phi(environment);
					psi(environment);
					predicted_UO(environment);
					predicted_UR(environment);
					predicted_SO(environment);
					predicted_SR(environment);
				}	
				apply_formula(&formula[1], 1);
				if(formula[1] != NO_ACTION)
				{
					phi(environment);
					psi(environment);
					predicted_UO(environment);
					predicted_UR(environment);
					predicted_SO(environment);
					predicted_SR(environment);
				}
			}
			else
			{
				if(sequence == SECOND)
				{
			
					apply_formula(&formula[1], 1);
					if(formula[1] != NO_ACTION)
					{
						phi(environment);
						psi(environment);
						predicted_UO(environment);
						predicted_UR(environment);
						predicted_SO(environment);
						predicted_SR(environment);
					}

					apply_formula(formula, 1);
					
					if(formula[0] != NO_ACTION)
					{
						phi(environment);
						psi(environment);
						predicted_UO(environment);
						predicted_UR(environment);
						predicted_SO(environment);
						predicted_SR(environment);
					}
				}
			}
		}	
		
		printf("%d\t%d",phi_satisfaction,psi_satisfaction);
		total++;
		//printf("Phi %d, Psi %d, total %d",phi_satisfaction, psi_satisfaction, total);
		switch(strategy)
		{
			case 0: printf("\t%d\n",UO_satisfaction); break;
			case 1: printf("\t%d\n",UR_satisfaction); break;
			case 2: printf("\t%d\n",SO_satisfaction); break;
			case 3: printf("\t%d\n",SR_satisfaction); break;
		}
	}

	
	return 0;
}


void apply_formula(int formula[], int length)
{
	int i;

	for(i = 0; i < length; i++)
	{
		switch(formula[i])
		{
			case A_TRUE: 
				environment |= 4;
				break;
			case B_TRUE: 
				environment |= 2;
				break;
			case A_FALSE: 
				environment &= ~(4);
				break;
			case B_FALSE: 
				environment &= ~(2);
				break;
			case C_TRUE: 
				environment |= 1;
				break;
			case C_FALSE: 
				environment &= ~(1);
				break;
			default: break;
		}
	}
}
