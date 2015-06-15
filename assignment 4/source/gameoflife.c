#include <stdio.h>
#include <mpi.h>

#define N 16

typedef struct cell{
   int type;
   int i;
   int j;
   int value;
} packet;

static void print_grid(int grid[N][N]);
static void calc_neighbors(int i, int j, int ra[]);

int proc[N][N];
int grid[N][N];
int sums[N][N]; 
int num_procs, rank, to_rank, from_rank, tag;

int main(int argc, char *argv[]) {
  int h, i, j, k, iter;
  int neighbors[8];
  MPI_Status status;
  packet temp_pack;
  packet *ptr_pack;
  if(MPI_Init(&argc, &argv) != MPI_SUCCESS)
  {
    printf("Please enter the correct amount of arguments\n");
  }
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(N % num_procs != 0)
  {
    if(rank == 0)
    {
      printf("Please enter 1, 2, 4, 8, or 16 processors\n");
    }
    MPI_Finalize();
    return 1;
  }
  k = 0;
  for(i = 0; i < N; i++)
  {
    for(j = 0; j < N; j++)
    {
      grid[i][j] = 0; 
      proc[i][j] = i/(N/num_procs);
      sums[i][j] = 0;
    }
  }
  grid[0][1] = 1;
  grid[1][2] = 1;
  grid[2][0] = 1;
  grid[2][1] = 1;
  grid[2][2] = 1;
  iter = 0;
  tag = 123;
  if(rank == 0)
  {
  printf("Initial setup:\n");
  print_grid(grid);
  }
  for(h = 0; h < 64; h++)
  {
    for(i = 0; i < N; i++)
    {
      for(j = 0; j < N; j++)
      {
        sums[i][j] = 0;
        to_rank = proc[i][j];
        calc_neighbors(i, j, neighbors);
        for(k = 0; k < 8; k++)
        {
          from_rank = neighbors[k];
          if(to_rank == rank)
          {
            if(from_rank == rank)
            {
              if(k == 0)
              { sums[i][j] += grid[(i-1+N) % N][(j-1+N) % N];}
              else if(k == 1)
              { sums[i][j] += grid[(i-1+N) % N][j];}
              else if(k == 2)
              { sums[i][j] += grid[(i-1+N) % N][(j+1+N) % N];}
              else if(k == 3)
              { sums[i][j] += grid[i][(j-1+N) % N];}
              else if(k == 4)
              { sums[i][j] += grid[i][(j+1+N) % N];}
              else if(k == 5)
              { sums[i][j] += grid[(i+1+N) % N][(j-1+N) % N];}
              else if(k == 6)
              { sums[i][j] += grid[(i+1+N) % N][j];}
              else if(k == 7)
              { sums[i][j] += grid[(i+1+N) % N][(j+1+N) % N];}
            }
            else
            {
              MPI_Recv(&temp_pack, sizeof(packet), MPI_BYTE, from_rank, tag, MPI_COMM_WORLD, &status);
              sums[i][j] += temp_pack.value;
            }
        }
        else
        {
          if(from_rank == rank)
          {
            temp_pack.type = 2;
            temp_pack.i = i;
            temp_pack.j = j;
              if(k == 0)
              { temp_pack.value = grid[(i-1+N) % N][(j-1+N) % N];}
              else if(k == 1)
              { temp_pack.value = grid[(i-1+N) % N][j];}
              else if(k == 2)
              { temp_pack.value = grid[(i-1+N) % N][(j+1+N) % N];}
              else if(k == 3)
              { temp_pack.value = grid[i][(j-1+N) % N];}
              else if(k == 4)
              { temp_pack.value = grid[i][(j+1+N) % N];}
              else if(k == 5)
              { temp_pack.value = grid[(i+1+N) % N][(j-1+N) % N];}
              else if(k == 6)
              { temp_pack.value = grid[(i+1+N) % N][j];}
              else if(k == 7)
              { temp_pack.value = grid[(i+1+N) % N][(j+1+N) % N];}
                MPI_Ssend(&temp_pack, sizeof(packet), MPI_BYTE, to_rank, tag, MPI_COMM_WORLD);
              }
            }
          }
        }
      }

  for(i = 0; i < N; i++)
  {
    for(j = 0; j < N; j++)
    {
      if(proc[i][j] == rank)  
      {
        if(grid[i][j] == 1)
        {
          if(sums[i][j] < 2 || sums[i][j] > 3)
          {
            grid[i][j] = 0;
          }
        }
        else
        {
          if(sums[i][j] == 3)
          {
            grid[i][j] = 1;
          }
        }
      if(rank != 0)
      {
        temp_pack.type = 3;
        temp_pack.i = i;
        temp_pack.j = j;
        temp_pack.value = grid[i][j];
        to_rank = 0;
        MPI_Ssend(&temp_pack, sizeof(packet), MPI_BYTE, to_rank, tag, MPI_COMM_WORLD);
      }
    }
    from_rank = proc[i][j];
    if(from_rank != 0)
    {
      if(rank == 0)
      {
        MPI_Recv(&temp_pack, sizeof(packet), MPI_BYTE, from_rank, tag, MPI_COMM_WORLD, &status);
        grid[i][j] = temp_pack.value;
      }
    }
  }
  }
  
  if(rank ==0)
  {
    printf("Iteration %d: updated grid\n", iter++);
    print_grid(grid);
  }
  }
  MPI_Finalize();
  return 0;
  }
  
  void print_grid(int grid[N][N]) {
  
  int i, j;
  for(i = 0; i < N; i++)
    {
      for(j = 0; j < N; j++)
      {
        printf("%d ", grid[i][j]);
      }
      printf("\n");
    }
  }
  
  void calc_neighbors(int i, int j, int temp[]) {
  
  temp[0] = proc[(i-1+N) % N][(j-1+N) % N];
  temp[1] = proc[(i-1+N) % N][j];
  temp[2] = proc[(i-1+N) % N][(j+1+N) % N];
  temp[3] = proc[i][(j-1+N) % N];
  temp[4] = proc[i][(j+1+N) % N];
  temp[5] = proc[(i+1+N) % N][(j-1+N) % N];
  temp[6] = proc[(i+1+N) % N][j];
  temp[7] = proc[(i+1+N) % N][(j+1+N) % N];
}
