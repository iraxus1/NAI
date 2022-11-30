import pandas as pd
from sklearn import tree
from sklearn.model_selection import train_test_split

diabetes_data = pd.read_csv('diabetes.csv')
diabetes_data.head()
diabetes_data['Outcome'].value_counts()
# Tworzenie zbioru cech X bez kolumny 'Outcome'
cols = set(diabetes_data.columns)
cols.remove('Outcome')
cols = list(cols)
X = diabetes_data[cols]

# Wyodrębnienie etykiet do klasyfikacji
y = diabetes_data['Outcome']

# Zbiór testowy i treningowy:
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3)
from collections import Counter

print(f"Training target statistics: {Counter(y_train)}")
print(f"Testing target statistics: {Counter(y_test)}")

# a. Zbalansuj zbiór nadpróbkowując mniej liczną klasę.
from imblearn.over_sampling import RandomOverSampler

ros = RandomOverSampler(random_state=0)
X_resampled, y_resampled = ros.fit_resample(X_train, y_train)
print(f"Training target statistics: {Counter(y_resampled)}")

# . Zbalansuj zbiór podpróbkowując liczniejszą klasę.
from imblearn.under_sampling import RandomUnderSampler

rus = RandomUnderSampler(random_state=0)
X_resampled, y_resampled = rus.fit_resample(X_train, y_train)
print(f"Training target statistics: {Counter(y_resampled)}")

# Sprawdzamy, czy zbiór jest zbalansowany po wykonanej operacji:
print(f"Training target statistics: {Counter(y_train)}")

# Stwórz dwa klasyfikatory typu Drzewo decyzyjne. Skorzystaj z biblioteki scikit-learn. Jeden klasyfikator wytrenuj na niezbalansowanym zbiorze, a drugi na zbalansowanym. Porównaj wyniki.

from sklearn import tree
from sklearn.metrics import accuracy_score

clf = tree.DecisionTreeClassifier()
clf.fit(X_train, y_train)
y_pred = clf.predict(X_test)
print(f"Accuracy score: {accuracy_score(y_test, y_pred)}")

# trening na danych zbalansowanych

clf = tree.DecisionTreeClassifier()
clf.fit(X_resampled, y_resampled)
y_pred = clf.predict(X_test)
print(f"Accuracy score: {accuracy_score(y_test, y_pred)}")

from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn.metrics import f1_score
from sklearn.metrics import accuracy_score

def print_scores(clf, X_test, y_test, header="Dataset statistics:"):
  y_pred = clf.predict(X_test)
  print('-------')
  print(header)
  print('\n')
  print('Precision score %s' % precision_score(y_test, y_pred))
  print('Recall score %s' % recall_score(y_test, y_pred))
  print('F1-score score %s' % f1_score(y_test, y_pred))
  print('Accuracy score %s' % accuracy_score(y_test, y_pred))

# Porównanie wyników:

print_scores(clf, X_test, y_test, header="Dataset statistics:")
print_scores(clf, X_resampled, y_resampled, header="Dataset statistics:")

# Wyrysuj stworzone drzewo decyzyjne. Czy uważasz, że wybrana przez algorytm struktura jest optymalna? Czy daje dobre wyniki?Wyrysuj stworzone drzewo decyzyjne. Czy uważasz, że wybrana przez algorytm struktura jest optymalna? Czy daje dobre wyniki?

from matplotlib import pyplot as plt
from sklearn import tree
import graphviz

dot_data = tree.export_graphviz(clf, out_file=None, feature_names=cols, class_names=['0', '1'], filled=True, rounded=True, special_characters=True)
graph = graphviz.Source(dot_data)
graph

# Jakie znasz metody na optymalizację struktury drzewa? Czemu pozwalają zapobiec? Wypisz tutaj te metody, a następnie wdróż je trenując nowy klasyfikator.
#
# (Twoja odpowiedź) Metody optymalizacji struktury drzewa:

# 1. Pruning - usuwanie gałęzi, które nie mają wpływu na wynik
# 2. Pruning - ograniczenie maksymalnej głębokości drzewa
# 3. Pruning - ograniczenie maksymalnej liczby liści
# 4. Pruning - ograniczenie maksymalnej liczby atrybutów

# Nowy klasyfikator - wytrenuj i wypisz wyniki
# Twój kod ...

# 1. Pruning - usuwanie gałęzi, które nie mają wpływu na wynik
clf = tree.DecisionTreeClassifier(ccp_alpha=0.01)
clf.fit(X_resampled, y_resampled)
y_pred = clf.predict(X_resampled)
print(f"1. Accuracy score of Pruning - usuwanie gałęzi, które nie mają wpływu na wynik: {accuracy_score(y_resampled, y_pred)}")

# 2. Pruning - ograniczenie maksymalnej głębokości drzewa
clf = tree.DecisionTreeClassifier(max_depth=3)
clf.fit(X_resampled, y_resampled)
y_pred = clf.predict(X_resampled)
print(f"2. Accuracy score of Pruning - usuwanie gałęzi, które nie mają wpływu na wynik: {accuracy_score(y_resampled, y_pred)}")

# 3. Pruning - ograniczenie maksymalnej liczby liści
clf = tree.DecisionTreeClassifier(max_leaf_nodes=3)
clf.fit(X_resampled, y_resampled)
y_pred = clf.predict(X_resampled)
print(f"3. Accuracy score of Pruning - ograniczenie maksymalnej głębokości drzewa: {accuracy_score(y_resampled, y_pred)}")

# 4. Pruning - ograniczenie maksymalnej liczby atrybutów
clf = tree.DecisionTreeClassifier(max_features=3)
clf.fit(X_resampled, y_resampled)
y_pred = clf.predict(X_resampled)
print(f"4. Accuracy score of Pruning - ograniczenie maksymalnej liczby atrybutów: {accuracy_score(y_resampled, y_pred)}")

# 5. Pruning - ograniczenie minimalnej głębokości drzewa
clf = tree.DecisionTreeClassifier(min_samples_leaf=3)
clf.fit(X_resampled, y_resampled)
y_pred = clf.predict(X_resampled)
print(f"5. Accuracy score of Pruning - ograniczenie minimalnej głębokości drzewa: {accuracy_score(y_resampled, y_pred)}")

# 6. Pruning - ograniczenie minimalnej liczby liści
clf = tree.DecisionTreeClassifier(min_samples_split=3)
clf.fit(X_resampled, y_resampled)
y_pred = clf.predict(X_resampled)
print(f"6. Accuracy score of Pruning - ograniczenie minimalnej liczby liści: {accuracy_score(y_resampled, y_pred)}")

# 7. Pruning - ograniczenie minimalnej liczby atrybutów
clf = tree.DecisionTreeClassifier(min_impurity_decrease=0.01)
clf.fit(X_resampled, y_resampled)
y_pred = clf.predict(X_resampled)
print(f"7. Accuracy score of Pruning - ograniczenie minimalnej liczby atrybutów: {accuracy_score(y_resampled, y_pred)}")



# Poeksperymentuj z parametrami związanymi z metodami, które wypisałeś_aś w poprzednim zadaniu. Wybierz najlepszy zestaw parametrów i po raz kolejny wyrysuj drzewo.

# Wyrysuj nowe drzewo
dot_data = tree.export_graphviz(clf, out_file=None, feature_names=cols, class_names=['0', '1'], filled=True, rounded=True, special_characters=True)
graph = graphviz.Source(dot_data)
graph


# Stwórz klasyfikator oparty o lasy losowe i sprawdź, jak polepszyło to klasyfikację Cukrzycy. Czy dobrane w poprzednim zadaniu parametry sprawdzają się przy lasach losowych?

from sklearn.ensemble import RandomForestClassifier

clf = RandomForestClassifier()
clf.fit(X_resampled, y_resampled)
y_pred = clf.predict(X_test)
print(f"Accuracy score: {accuracy_score(y_test, y_pred)}")

# Znajdź na Kaggle zbiór danych, w których znajdują się zmienne kategoryczne. Czy drzewo decyzyjne w scikit-learn radzi sobie z takimi wartościami? Jeżeli nie, jak można obejść ten problem? Napisz odpowiednią funkcję.

def categorical_to_numerical(df):
    df = df.copy()
    for col in df.columns:
        if df[col].dtype == 'object':
            df[col] = df[col].astype('category')
            df[col] = df[col].cat.codes
    return df

# Znajdź na Kaggle lub w scikit-learn zbiór danych do regresji (np. House Prices prediction). Pobierz go i zbuduj drzewo regresyjne.

# Data frame z danymi oraz drzewo regresji
# Twój kod ...

# Znajdź na Kaggle zbiór danych, w których znajdują się zmienne kategoryczne. Czy drzewo decyzyjne w scikit-learn radzi sobie z takimi wartościami? Jeżeli nie, jak można obejść ten problem? Napisz odpowiednią funkcję.

from sklearn.datasets import load_boston
from sklearn.tree import DecisionTreeRegressor
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error

def categorical_to_numerical(df):
    df_mod = pd.Dataframe()
  # '''
  #   INPUT: Data frame of feature values. It may contain categorical variables.
  #   OUTPUT: Modified data frame with categorical variables encoded as numerical.
  # '''
    # Twój kod ...
    for col in df.columns:
        if df[col].dtype == 'object':
            df[col] = df[col].astype('category')
            df[col] = df[col].cat.codes
    return df
































