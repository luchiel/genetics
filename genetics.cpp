#include <cstdio>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

std::vector< std::vector<int> >& generate_population(int size, int gene_count, int value_limit)
{
    std::vector< std::vector<int> >* result = new std::vector< std::vector<int> >();

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, value_limit - 1);

    for(int i = 0; i < size; ++i)
    {
        std::vector<int> gene(gene_count, 0);
        for(int j = 0; j < gene_count; ++j)
            gene[j] = distribution(generator);

        result->push_back(gene);
    }
    return *result;
}

int number_of_problems;
std::vector<int> complexity;
std::vector<double> time;
std::vector< std::vector<double> > coefficients;
int number_of_workers;

void read_problem_conditions()
{
    freopen("input.txt", "r", stdin);
    scanf("%d\n", &number_of_problems);
    complexity.resize(number_of_problems);
    for(int i = 0; i < number_of_problems; ++i)
    {
        scanf("%d", &complexity[i]);
        complexity[i]--;
        if(i != number_of_problems - 1)
            scanf(", ");
    }
    scanf("\n");
    time.resize(number_of_problems);
    for(int i = 0; i < number_of_problems; ++i)
    {
        scanf("%lf", &time[i]);
        if(i != number_of_problems - 1)
            scanf(", ");
    }
    scanf("\n%d\n", &number_of_workers);
    coefficients.resize(number_of_workers);
    for(int i = 0; i < number_of_workers; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            double temp;
            scanf("%lf", &temp);
            if(j != 3)
                scanf(", ");
            coefficients[i].push_back(temp);
        }
        if(i != number_of_workers - 1)
            scanf("\n");
    }
}

struct Settings
{
    int population_size;
};

double get_gene_survival_coefficient(std::vector<int>& gene)
{
    double maximal_time = 0.0;
    std::vector<double> workers(number_of_workers, 0.0);
    for(int task_number = 0; task_number < gene.size(); ++task_number)
    {
        int task_complexity = complexity[task_number];
        double task_time = time[task_number];

        int worker = gene[task_number];
        double spent_time = coefficients[worker][task_complexity] * task_time;

        workers[worker] += spent_time;
        if(maximal_time < workers[worker])
            maximal_time = workers[worker];
    }
    return maximal_time;
}

int main()
{
    read_problem_conditions();
    Settings settings = { number_of_problems };

    std::vector< std::vector<int> >& population =
        generate_population(settings.population_size, number_of_problems, number_of_workers);

    std::vector< std::pair<double, int> > gene_survival;
    for(int i = 0; i < settings.population_size; ++i)
        gene_survival.push_back(std::make_pair(get_gene_survival_coefficient(population[i]), i));

    std::sort(gene_survival.begin(), gene_survival.end());

    return 0;
}
