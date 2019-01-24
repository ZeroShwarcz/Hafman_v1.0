#include<map>
#include<vector>
#include<list>
#include<string>
#include<iostream>
#include<fstream>


using namespace std;

class Uzel
{
    public:
        char c;
        int a;
        Uzel *left, *right;     //указатель на левого и правого сына

        Uzel(){}             // перегрузка конструктора(избежание возможных проблем)

        Uzel (Uzel* L, Uzel* R)      // конструктор для суммы сыновей (родитель)
        {
            left = L;               // замена
            right = R;              // замена
            a = L -> a + R -> a;    // сумма двух переменных
        }
};


/* класс my_compare отвечает за сортировку нашего списка */
struct my_compare
{
    bool operator()(Uzel* l, Uzel* r) const
    {
        return l -> a < r -> a;      // сортировка по возрастанию
    }
};
/*
void Print(Uzel* root, unsigned k = 0)
{
    if(root != NULL)
    {
        Print(root -> left, k + 3);          //рекурсивная функция для вывода левого сына

        for (unsigned i = 0; i < k; i++ )
        {
            cout<< "    ";
        }

        if(root -> c)
            cout << root -> a << "(" << root -> c << ")" <<endl;
        else
            cout << root -> a << endl;
        
        Print(root -> right, k+3);          //рекурсивная функция для вывода правого сына
    }
}
*/

vector<bool> code;                  // код из 0 и 1
map<char, vector<bool> > table;     // ассоциация символа с кодом


void build_table(Uzel* root)
{
    if (root -> left != NULL)           // если слева не 0, то идём по левому ребру
    {
        code.push_back(0);              // ставим 0
        build_table(root -> left);      // рекурсивная функция для левого сына
    }
    
    if (root -> right != NULL)          // если справа не 0, то идём по правому ребру
    {
        code.push_back(1);              // ставим 1
        build_table(root -> right);     // рекурсивная функция для правого сына
    }
    
    if(root -> c)
        table[root -> c] = code;        // если нашли букву, то ассоциируем её с кодом
    
    code.pop_back();                    // сокращаем код на единицу
    
}

void input_data()
{
    
}

int Hafman()
{
    ifstream f("input.txt");       // файл с входными данными

    map<char,int> m;

    while(!f.eof())   //цикл для подсчёта вхождения каждого символа
    {
        char c = f.get();             //записываем в переменную "с" данных из файла
        m[c]++;             //увеличиваем переменную m от с на еденицу
    }


        list<Uzel* > t;

        map<char, int>::iterator itr;
       /* for (itr = m.begin(); itr != m.end(); ++itr)
        {
            // при помощи итератора itr обращаемся к первому(key = first) и второму(key = second) элементу ассоциативного массива m
            cout << itr -> first << ":" << itr -> second << endl;
        }
        */
        /* цикл для записи значений из m в узел */
        for (itr = m.begin(); itr != m.end(); ++itr)
        {
            Uzel *p = new Uzel;         // создаём в динамической памяти новый узел
            p -> c = itr -> first;      // переменная узла с становится первым элементом ассоциативного массива m
            p -> a = itr -> second;     // переменная узла а становится вторым элементом ассоциативного массива m
            t.push_back(p);             // указатель на этот узел сохраняем в лист
        }


        while(t.size() != 1)             // создание дерева
        {
            t.sort(my_compare());

            Uzel *L_son = t.front();     // вспомогательному элементу L_son присваеваем значение первого элемента списка
            t.pop_front();               // удаляем этот первый элемент, на его место становится второй элемент списка
            Uzel *R_son = t.front();     // нынешний элемент стоящий на первом месте
            t.pop_front();               // удаление этого элемента

            Uzel *parent = new Uzel(L_son, R_son);  // создаём отца предыдущих 2х элементов
            t.push_back(parent);                 // сохраняем отца в список
        }



        Uzel *root = t.front();          // корневой элемент дерева

        build_table(root);
        
        f.clear();              // сброс "ошибок"
        f.seekg(0);             //перещение указателя на 0
        
        ofstream g("output.bin");   // вывод результата в файл
        int count = 0;          // счётчик
        char buff = 0;          // вспомогательная переменная
        
        while(!f.eof())       //проходим по строке
        {
            char c = f.get();                          // вспомогательная переменная с
            vector<bool> x = table[c];              // массив содержащий код соответствующей буквы
            
            for (int j = 0; j < x.size(); j++ )     // вывод вектора х на экран
            {
                buff = buff | x[j] << (7 - count);  // преобразование вектора в байт(8 бит)
                count++;
                if(count == 8)                      // если счётчик = 8 битам
                {
                    count = 0;                      // обнуляем счётчик
                    g << buff;                      // записываем получившийся байт в файл
                    buff = 0;                       // обнуление buff
                }
            }
        }
        cout << endl;
        
        g.close();
        f.close();

        ifstream Fin("output.bin");
        
        Uzel *p = root;         // указатель на корневой элемент
        count = 0;              // счётчик
        char byte;              // вспомогательная переменная для байта
        Fin >> byte;            // считывание байта из файла
        
        while(!Fin.eof())       // пока не конец файла 
        {
            bool b = byte & (1 << (7 - count) );        // проверка первого бита
            if(b)
                p = p -> right;                         // если бит = 1, то идём по правому сыну
            else
                p = p -> left;                          // иначе по левому
            if(p -> left == NULL && p -> right == NULL)   // поверка на конец сына
            {
                cout << p -> c;                         // вывести букву
                p = root;                               
            }
            count++;
            if(count == 8)                              // если счётчик = 8 бит
            {                                           
                count = 0;                              // обнуляем счётчик
                Fin >> byte;                            // считываем новый символ
            }
        }
        Fin.close();

}
