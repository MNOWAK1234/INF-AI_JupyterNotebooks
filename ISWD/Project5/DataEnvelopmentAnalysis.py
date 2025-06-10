import pandas as pd
import numpy as np
from pulp import LpProblem, LpVariable, LpMaximize, LpMinimize, value, PULP_CBC_CMD


def efficiency_model(inputs, outputs, names, input_dmu, output_dmu, dmu_name, supereffective = False):
    '''
    Funkcja definiująca problem programowania matematycznego dla modelu CCR zorientowanego na nakłady
    w przestrzeni efektywności.
    '''
    problem = LpProblem("efficiency", LpMaximize)

    input_weights = {}
    for name, _ in inputs.items():
        input_weights[name] = LpVariable(name=name, lowBound=0)

    output_weights = {}
    for name, _ in outputs.items():
        output_weights[name] = LpVariable(name=name, lowBound=0)
    
    problem += sum([output_dmu[name] * output_weights[name] for name, _ in outputs.items()])

    problem += sum([input_dmu[name] * input_weights[name] for name, _ in inputs.items()]) == 1

    for (_, input_k), (_, output_k), name_k in zip(inputs.iterrows(), outputs.iterrows(), names):
        if supereffective and name_k == dmu_name:
            continue

        input_sum = sum([output_k[name] * output_weights[name] for name, _ in outputs.items()])
        output_sum = sum([input_k[name] * input_weights[name] for name, _ in inputs.items()])
        problem += input_sum <= output_sum

    return problem, input_weights, output_weights


def combination_model(inputs, outputs, names, input_dmu, output_dmu, dmu_name):
    '''
    Funkcja definiująca problem programowania matematycznego dla modelu CCR zorientowanego na nakłady
    w przestrzeni kombinacji.
    '''
    problem = LpProblem("combination", LpMinimize)
    theta = LpVariable(name="theta", lowBound=0)
    problem += theta

    variables = {}
    for name in names:
        variables[name] = LpVariable(name=name, lowBound=0)

    for column, _ in inputs.items():
        input_sum = sum([variables[name] * input[column] for name, (_, input) in zip(names, inputs.iterrows())])
        problem += input_sum <= theta * input_dmu[column]

    for column, _ in outputs.items():
        output_sum = sum([variables[name] * output[column] for name, (_, output) in zip(names, outputs.iterrows())])
        problem += output_sum >= output_dmu[column]

    return problem, variables


# wczytanie danych
inputs = pd.read_csv('inputs.csv', sep=';')
outputs = pd.read_csv('outputs.csv', sep=';')
samples = pd.read_csv('samples_homework.csv', sep=';')

names = inputs['Unnamed: 0']
inputs = inputs.drop(columns=inputs.columns[0])
outputs = outputs.drop(columns=outputs.columns[0])

# rozwiązanie modelu dla każdej jednostki
# wyniki: efektywność, HCU, superefektywność
super_efficiencies = []
all_input_weights = []
all_output_weights = []

for (_, input_dmu), (_, output_dmu), dmu_name in zip(inputs.iterrows(), outputs.iterrows(), names):
    efficiency_problem, input_weights, output_weights = efficiency_model(inputs, outputs, names, input_dmu, output_dmu, dmu_name)
    combination_problem, variables = combination_model(inputs, outputs, names, input_dmu, output_dmu, dmu_name)
    superefficiency_problem, _, _ = efficiency_model(inputs, outputs, names, input_dmu, output_dmu, dmu_name, supereffective=True)

    efficiency_problem.solve(PULP_CBC_CMD(msg=False))
    combination_problem.solve(PULP_CBC_CMD(msg=False))
    superefficiency_problem.solve(PULP_CBC_CMD(msg=False))

    print(f"Showing result for {dmu_name}")

    efficiency = round(value(combination_problem.objective), 3)
    print(f"Efficiency: {efficiency}")

    if efficiency == 1:
        print(f"{dmu_name} is efficient")
    else:
        print(f"{dmu_name} is not efficient")

        for column, _ in inputs.items():
            hcu = sum([value(variables[name]) * input[column] for name, (_, input) in zip(names, inputs.iterrows())])
            diff = input_dmu[column] - hcu
            print(f"{column} HCU: {round(hcu, 3)}, diff: {round(diff, 3)}")

        for column, _ in outputs.items():
            hcu = sum([value(variables[name]) * output[column] for name, (_, output) in zip(names, outputs.iterrows())])
            diff = hcu - output_dmu[column]
            print(f"{column} HCU: {round(hcu, 3)}, diff: {round(diff, 3)}")

    superefficiency = value(superefficiency_problem.objective)
    super_efficiencies.append((dmu_name, superefficiency))
    print(f"")  

    all_input_weights.append(input_weights)
    all_output_weights.append(output_weights)     

# ranking superfektywności 
super_efficiencies.sort(key=lambda dmu: dmu[1], reverse=True)

print(f"\nSuper-efficiency ranking:") 
for i, (name, score) in enumerate(super_efficiencies, start=1):
    print(f"{i}. {name}: {round(score, 3)}")


# efektywność krzyżowa
print(f"\nCross-efficiency matrix:") 
cross_efficiencies = [] 
for (_, input_dmu), (_, output_dmu), dmu_name in zip(inputs.iterrows(), outputs.iterrows(), names):
    print(dmu_name, end=" ")
    row_sum = 0
    for input_weights, output_weights in zip(all_input_weights, all_output_weights):
        sum_inputs = sum([value(input_weights[column]) * input_dmu[column] for column, _ in inputs.items()])
        sum_outputs = sum([value(output_weights[column]) * output_dmu[column] for column, _ in outputs.items()])
        result = sum_outputs / sum_inputs
        print(round(result, 3), end=" & ")
        row_sum += result
    average = row_sum/len(names)
    print(round(average, 3))
    cross_efficiencies.append((dmu_name, average))

cross_efficiencies.sort(key=lambda dmu: dmu[1], reverse=True)

print(f"\nCross-efficiency ranking:") 
for i, (name, score) in enumerate(cross_efficiencies, start=1):
    print(f"{i}. {name}: {round(score, 3)}")


# analiza Monte Carlo
buckets = ["0-0.2", "0.2-0.4", "0.4-0.6", "0.6-0.8", "0.8-1"]
efficiency_distribution = pd.DataFrame(0, index=pd.Series(names), columns=buckets) 

average_efficiencies = {}
for name in names:
    average_efficiencies[name] = 0

for _, sample in samples.iterrows():
    # efektywność jednostki
    efficiencies = []
    for (_, input_dmu), (_, output_dmu), dmu_name in zip(inputs.iterrows(), outputs.iterrows(), names):
        sum_inputs = sum([sample[column] * input_dmu[column] for column, _ in inputs.items()])
        sum_outputs = sum([sample[column] * output_dmu[column] for column, _ in outputs.items()])
        efficiencies.append(sum_outputs/sum_inputs)

    # normalizacja: podział przez maksimum
    max_eff = max(efficiencies)
    efficiencies = [eff / max_eff for eff in efficiencies]

    for efficiency, name in zip(efficiencies, names):
        average_efficiencies[name] += efficiency

        if efficiency < 0.2:
            efficiency_distribution.at[name, "0-0.2"] += 1
        elif efficiency < 0.4:
            efficiency_distribution.at[name, "0.2-0.4"] += 1
        elif efficiency < 0.6:
            efficiency_distribution.at[name, "0.4-0.6"] += 1
        elif efficiency < 0.8:
            efficiency_distribution.at[name, "0.6-0.8"] += 1
        else:
            efficiency_distribution.at[name, "0.8-1"] += 1

# rozkład efektywności
print(f"\nEfficiency distribution:") 
efficiency_distribution = efficiency_distribution.div(len(samples))
efficiency_distribution = efficiency_distribution.round(3)
print(efficiency_distribution)

# średnia efektywność
for name in names:
    average_efficiencies[name] /= len(samples)
average_efficiencies = {k: v for k, v in sorted(average_efficiencies.items(), key=lambda item: item[1], reverse=True)}

print(f"\nExpected effciency ranking:") 
for i, (name, score) in enumerate(average_efficiencies.items(), start=1):
    print(f"{i}. {name}: {round(score, 3)}")