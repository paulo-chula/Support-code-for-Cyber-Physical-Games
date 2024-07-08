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


//Game environment: boolean variables A  and B
//!A!B = 00
//!AB = 01
// etc 
int environment = 0;


//allocates a random number generator "victory", used for uniform victory generation
gsl_rng *rng;


/*
	LTL formulae "classes"

	We expect evaluation ("apply_formula" function) calls the update function at each environment update time step
*/
#define UNDECIDED 	0
#define FAILED 		1
#define SUCCESSFUL	2

struct LTL_formula
{
	int state;
	int state_matrix[4][4];
	int score;
} phi, psi;

void init_LTL_phi()
{
	phi.state_matrix[0][0] = UNDECIDED;
	phi.state_matrix[0][1] = FAILED;
	phi.state_matrix[0][2] = UNDECIDED;
	phi.state_matrix[0][3] = FAILED;

	phi.state_matrix[1][0] = FAILED;
	phi.state_matrix[1][1] = FAILED;
	phi.state_matrix[1][2] = FAILED;
	phi.state_matrix[1][3] = FAILED;

	phi.state_matrix[2][0] = FAILED;
	phi.state_matrix[2][1] = FAILED;
	phi.state_matrix[2][2] = FAILED;
	phi.state_matrix[2][3] = SUCCESSFUL;

	phi.state_matrix[3][0] = FAILED;
	phi.state_matrix[3][1] = FAILED;
	phi.state_matrix[3][2] = FAILED;
	phi.state_matrix[3][3] = FAILED;

	phi.state = 0;
	phi.score = UNDECIDED;
}

void init_LTL_psi()
{
	psi.state_matrix[0][0] = FAILED;
	psi.state_matrix[0][1] = UNDECIDED;
	psi.state_matrix[0][2] = FAILED;
	psi.state_matrix[0][3] = FAILED;

	psi.state_matrix[1][0] = UNDECIDED;
	psi.state_matrix[1][1] = UNDECIDED;
	psi.state_matrix[1][2] = UNDECIDED;
	psi.state_matrix[1][3] = SUCCESSFUL;

	psi.state_matrix[2][0] = UNDECIDED;
	psi.state_matrix[2][1] = UNDECIDED;
	psi.state_matrix[2][2] = UNDECIDED;
	psi.state_matrix[2][3] = SUCCESSFUL;

	psi.state_matrix[3][0] = FAILED;
	psi.state_matrix[3][1] = FAILED;
	psi.state_matrix[3][2] = FAILED;
	psi.state_matrix[3][3] = FAILED;

	psi.state = 0;
	psi.score = UNDECIDED;
}


void update_LTL(struct LTL_formula *f, int n_state)
{
	//printf("Update with state %d: prior %d\n",n_state,f->state);

	if((f->score == SUCCESSFUL) || (f->score == FAILED))
		return;

	f->score = f->state_matrix[f->state][n_state];

	//printf("\t Score %d\n",f->score);

	f->state = n_state;
}




//Applies a formula of given length to the environment
void apply_formula(int formula[], int length);


//returns an equally distributed random permutation of a sequence of actions
void permutation(int sequence[], int length);



int main(int argc, char *argv[])
{
	int strategy;

	if(argc != 2)
	{
		printf("Error: use \"<program name> <agent strategy>\"\n");
		return 0;
	}

	int phi_score = 0;
	int psi_score = 0;

	int total = 0;

	//allocates a random number generator "victory", used for uniform victory generation
	rng = gsl_rng_alloc (gsl_rng_taus);
	//seeds the random number generator
	gsl_rng_set(rng, time(NULL));


	strategy = atoi(argv[1]);

	printf("Phi\tPsi\n");
	//The collaborative case is simple enough that we can hardcode behavior
	switch(strategy)
	{
	case UO:{
				int sequence[4] = {A_TRUE,B_TRUE,A_TRUE,B_TRUE};

				int i;

				

				for(i=0; i < 10000; i++)
				{
					permutation(sequence, 4);
					environment = 0;

					init_LTL_phi();
					init_LTL_psi();

					apply_formula(sequence, 4);

					total++;
					if(phi.score == SUCCESSFUL)
						phi_score++;
					if(psi.score == SUCCESSFUL)
						psi_score++;

					printf("%d\t%d\n",phi_score,psi_score);
				}
				//printf("Phi %d, Psi %d\n",phi.score, psi.score);
				//printf("UO - Phi %d, Psi %d, total %d\n",phi_score, psi_score, total);
			} break;
	case UR:{
				//permute first two

				//if b is true
					//b true,  a false (length 4 sequence)

				//else
					//b true (length 3 sequence)
				int sequence[4] = {A_TRUE,B_TRUE, B_TRUE, A_FALSE};

				int i;
				for(i=0; i < 10000; i++)
				{
					permutation(sequence, 2);
					environment = 0;

					init_LTL_phi();
					init_LTL_psi();

					apply_formula(sequence, 2);

					if(environment & 1)
					{
						permutation(&sequence[2], 2);
						apply_formula(&sequence[2], 2);
					}
					else
					{
						apply_formula(&sequence[2], 2);
						phi.score = FAILED;
					}

					total++;
					if(phi.score == SUCCESSFUL)
						phi_score++;
					if(psi.score == SUCCESSFUL)
						psi_score++;

					printf("%d\t%d\n",phi_score,psi_score);
				}
				//printf("UO - Phi %d, Psi %d, total %d\n",phi_score, psi_score, total);
			} break;
	case SO:{ 
				int sequence[4] = {A_TRUE,B_TRUE,B_FALSE,B_TRUE};

				int i;
				for(i=0; i < 10000; i++)
				{
					permutation(sequence, 4);
					environment = 0;

					init_LTL_phi();
					init_LTL_psi();

					apply_formula(sequence, 4);

					total++;
					if(phi.score == SUCCESSFUL)
						phi_score++;
					if(psi.score == SUCCESSFUL)
						psi_score++;

					printf("%d\t%d\n",phi_score,psi_score);
				}
				//printf("Phi %d, Psi %d\n",phi.score, psi.score);
				//printf("SO - Phi %d, Psi %d, total %d\n",phi_score, psi_score, total);
			} break;
	case SR: {
				int sequence[2] = {A_TRUE, B_TRUE};
				int i;
				for(i=0; i < 10000; i++)
				{
					
					environment = 0;

					init_LTL_phi();
					init_LTL_psi();

					
					environment |= 1;
					update_LTL(&phi, environment);
					update_LTL(&psi, environment);
					environment &= ~(1);
					update_LTL(&phi, environment);
					//this is a hack, but it's the easiest way to get formular satisfied despte back to 0.
					//easier than re-writing the LTL logic...
					init_LTL_phi();


					permutation(sequence, 2);
					apply_formula(sequence, 2);

					/*
					environment |= 2;
					update_LTL(&phi, environment);
					update_LTL(&psi, environment);
					environment |= 1;
					update_LTL(&phi, environment);
					update_LTL(&psi, environment);
					*/
					

					total++;
					if(phi.score == SUCCESSFUL)
						phi_score++;
					if(psi.score == SUCCESSFUL)
						psi_score++;

					printf("%d\t%d\n",phi_score,psi_score);
				}
				//printf("Phi %d, Psi %d\n",phi.score, psi.score);
				//printf("SO - Phi %d, Psi %d, total %d\n",phi_score, psi_score, total);
	}break;
	}

	return 0;
}



/*
	Formulae:

		C0: (00) -> F(10) -> X(11) (phi)
		C1: (00) -> X(01) -> F(11) (psi)
*/
void apply_formula(int formula[], int length)
{
	int i;

	for(i = 0; i < length; i++)
	{
		switch(formula[i])
		{
			case A_TRUE: 
				environment |= 2;
				update_LTL(&phi, environment);
				update_LTL(&psi, environment);
				break;
			case B_TRUE: 
				environment |= 1;
				update_LTL(&phi, environment);
				update_LTL(&psi, environment);
				break;
			case A_FALSE: 
				environment &= ~(2);
				update_LTL(&phi, environment);
				update_LTL(&psi, environment);
				break;
			case B_FALSE: 
				environment &= ~(1);
				update_LTL(&phi, environment);
				update_LTL(&psi, environment);
				break;
		}
	}
}

// A utility function to swap to integers
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

//returns an equally distributed random permutation of a sequence of actions
void permutation(int sequence[], int length)
{

 
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = length-1; i > 0; i--)
    {
    	// Pick a random index from 0 to i
    	unsigned int j = gsl_rng_get(rng);
    	j %= (i+1);

        
        //int j = rand() % (i+1);
 
        // Swap arr[i] with the element at random index
        swap(&sequence[i], &sequence[j]);
    }
}