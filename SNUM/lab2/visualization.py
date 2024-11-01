import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def main():
    # Load data from CSV file
    df = pd.read_csv('cross.csv')  # Update with your actual CSV filename

    # Calculate metrics based on the confusion matrix
    df[['TP', 'FP', 'FN', 'TN']] = df['Confusion Matrix'].apply(
        lambda x: pd.Series(eval(x.replace('"', ''))))
    
    # Calculate accuracy, precision, recall, and F1 score
    df['Accuracy'] = (df['TP'] + df['TN']) / (df['TP'] + df['FP'] + df['FN'] + df['TN'])
    df['Precision'] = df['TP'] / (df['TP'] + df['FP'])
    df['Recall'] = df['TP'] / (df['TP'] + df['FN'])
    df['F1 Score'] = 2 * (df['Precision'] * df['Recall']) / (df['Precision'] + df['Recall'])

    # Print the DataFrame to console
    print(df)

    # Find the best result for the test set
    best_result_index = df['Accuracy'].idxmax()
    best_result = df.iloc[best_result_index]

    print("\nBest result based on test set:")
    print(best_result[['Confidence Factor', 'minNumObj', 'Binary Splits', 'Accuracy', 'Precision', 'Recall', 'F1 Score']])

    # Plotting metrics against Confidence Factor
    plt.figure(figsize=(12, 8))
    
    # Plotting for Confidence Factor
    plt.subplot(2, 2, 1)
    plt.plot(df['Confidence Factor'], df['Accuracy'], marker='o', linestyle='None')  # No line
    plt.title('Accuracy vs Confidence Factor')
    plt.xlabel('Confidence Factor')
    plt.ylabel('Accuracy')
    
    plt.subplot(2, 2, 2)
    plt.plot(df['Confidence Factor'], df['Precision'], marker='o', color='orange', linestyle='None')  # No line
    plt.title('Precision vs Confidence Factor')
    plt.xlabel('Confidence Factor')
    plt.ylabel('Precision')
    
    plt.subplot(2, 2, 3)
    plt.plot(df['Confidence Factor'], df['Recall'], marker='o', color='green', linestyle='None')  # No line
    plt.title('Recall vs Confidence Factor')
    plt.xlabel('Confidence Factor')
    plt.ylabel('Recall')

    plt.subplot(2, 2, 4)
    plt.plot(df['Confidence Factor'], df['F1 Score'], marker='o', color='red', linestyle='None')  # No line
    plt.title('F1 Score vs Confidence Factor')
    plt.xlabel('Confidence Factor')
    plt.ylabel('F1 Score')

    plt.tight_layout()
    plt.show()

    # Plotting for minNumObj
    plt.figure(figsize=(12, 8))

    plt.subplot(2, 2, 1)
    plt.scatter(df['minNumObj'], df['Accuracy'], marker='o')  # Using scatter
    plt.title('Accuracy vs minNumObj')
    plt.xlabel('minNumObj')
    plt.ylabel('Accuracy')

    plt.subplot(2, 2, 2)
    plt.scatter(df['minNumObj'], df['Precision'], marker='o', color='orange')  # Using scatter
    plt.title('Precision vs minNumObj')
    plt.xlabel('minNumObj')
    plt.ylabel('Precision')

    plt.subplot(2, 2, 3)
    plt.scatter(df['minNumObj'], df['Recall'], marker='o', color='green')  # Using scatter
    plt.title('Recall vs minNumObj')
    plt.xlabel('minNumObj')
    plt.ylabel('Recall')

    plt.subplot(2, 2, 4)
    plt.scatter(df['minNumObj'], df['F1 Score'], marker='o', color='red')  # Using scatter
    plt.title('F1 Score vs minNumObj')
    plt.xlabel('minNumObj')
    plt.ylabel('F1 Score')

    # Set integer ticks for minNumObj plots
    for ax in plt.gcf().axes:
        ax.set_xticks(range(int(df['minNumObj'].min()), int(df['minNumObj'].max()) + 1))
        ax.xaxis.set_major_locator(plt.MaxNLocator(integer=True))

    plt.tight_layout()
    plt.show()

    # Plotting for Binary Splits
    plt.figure(figsize=(12, 8))

    plt.subplot(2, 2, 1)
    plt.scatter(df['Binary Splits'], df['Accuracy'], marker='o')  # Using scatter
    plt.title('Accuracy vs Binary Splits')
    plt.xlabel('Binary Splits')
    plt.ylabel('Accuracy')

    plt.subplot(2, 2, 2)
    plt.scatter(df['Binary Splits'], df['Precision'], marker='o', color='orange')  # Using scatter
    plt.title('Precision vs Binary Splits')
    plt.xlabel('Binary Splits')
    plt.ylabel('Precision')

    plt.subplot(2, 2, 3)
    plt.scatter(df['Binary Splits'], df['Recall'], marker='o', color='green')  # Using scatter
    plt.title('Recall vs Binary Splits')
    plt.xlabel('Binary Splits')
    plt.ylabel('Recall')

    plt.subplot(2, 2, 4)
    plt.scatter(df['Binary Splits'], df['F1 Score'], marker='o', color='red')  # Using scatter
    plt.title('F1 Score vs Binary Splits')
    plt.xlabel('Binary Splits')
    plt.ylabel('F1 Score')

    # Set integer ticks for Binary Splits plots
    for ax in plt.gcf().axes:
        ax.set_xticks(range(int(df['Binary Splits'].min()), int(df['Binary Splits'].max()) + 1))
        ax.xaxis.set_major_locator(plt.MaxNLocator(integer=True))

    plt.tight_layout()
    plt.show()

    # Plotting confusion matrix for all parameters
    plt.figure(figsize=(12, 8))
    confusion_matrices = np.array(df[['TP', 'FP', 'FN', 'TN']].values.tolist())
    
    # Plotting
    for i, (TP, FP, FN, TN) in enumerate(confusion_matrices):
        plt.subplot(4, 4, i + 1)  # Adjusting subplot grid based on number of parameters
        plt.imshow([[TP, FP], [FN, TN]], cmap='Blues', interpolation='nearest')
        plt.title(f'Conf Matrix\nCF: {df["Confidence Factor"][i]}, Obj: {df["minNumObj"][i]}, Splits: {df["Binary Splits"][i]}')
        plt.colorbar()
        plt.xticks([0, 1], ['Pred Pos', 'Pred Neg'])
        plt.yticks([0, 1], ['True Pos', 'True Neg'])

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()
