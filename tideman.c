#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void create_cycle(int i);
void get_winner(int i);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    // lock(int n)
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (ranks[i] == j)
            {
                for (int count = i + 1; count < candidate_count; count++)
                {
                    preferences[ranks[i]][ranks[count]]++;
                }
            }
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            else if (preferences[i][j] == preferences[j][i])
            {
                continue;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    pair temp[MAX];
    int count = 0;
    int highest_position;
    for (int j = 0; j < candidate_count; j++)
    {
        int highest = 0;

        for (int i = 0; i < candidate_count; i++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] > highest)
            {
                highest = preferences[pairs[i].winner][pairs[i].loser];
                temp[j].winner = pairs[i].winner;
                temp[j].loser = pairs[i].loser;
                highest_position = i;
            }
        }
        preferences[pairs[highest_position].winner][pairs[highest_position].loser] = preferences[pairs[highest_position].winner][pairs[highest_position].loser] * -1;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        pairs[i].winner = temp[i].winner;
        pairs[i].loser = temp[i].loser;
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    /*for(int i =0;i<pair_count;i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
    }*/
    create_cycle(0);
    return;
}

// Print the winner of the election

void print_winner(void)
{
    bool the_winner;
    int winner[MAX];
    int count = 0;

    for (int j = 0; j < candidate_count; j++)
    {
        the_winner = true;
        for (int i = 0; i < candidate_count; i++)
        {
            if (i == j)
            {
                continue;
            }
            if (locked[i][j])
            {
                the_winner = false;
                break;
            }
        }
        if (the_winner == true)
        {
            printf("%s\n", candidates[j]);
        }
    }
    /*for(int k = 0;k<count;k++)
    {
        printf("%s\n",candidates[winner[k]]);
    }*/
}

void create_cycle(int i)
{
    if (i > pair_count - 1)
    {
        return;
    }
    int check1 = 0;
    int check2 = 0;
    int check3 = 0;
    int checker1;
    int checker2;
    int winner = pairs[i].winner;
    int loser = pairs[i].loser;
    for (int j = 0; j < i; j++)
    {
        if (locked[pairs[j].winner][pairs[j].loser])
        {
            if (pairs[j].winner == loser)
            {
                checker1 = pairs[j].loser;
                check1++;
            }
            if (pairs[j].loser == winner)
            {
                checker2 = pairs[j].winner;
                check2++;
            }
            if (checker1 == checker2)
            {
                check3++;
            }
        }
    }
    if (check1 > 0 && check2 > 0 && check3 > 0)
    {
        locked[pairs[i].winner][pairs[i].loser] = false;
    }
    else
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
    }
    create_cycle(i + 1);
}
