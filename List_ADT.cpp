// Убрать повторяющиеся элементы из списка рассылки, содержащего имена и адреса (список реализовать разными способами)
#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

// Объект списка
struct object 
{
	char name[20];
	char address[30];
	
	int operator == (object& x) const { return (strcmp(name, x.name)==0 && strcmp(address, x.address)==0); }
	int operator != (object& x) const { return !(*this==x); }
	
	void print()
	{
		cout<<name<<" "<<address<<endl;
	}                                       
};


/* Реализация списка на массиве */ 

namespace array_based_list
{
	typedef int pos;
	
	struct elem
	{
		object ob;
		pos next;
		void init()
		{
			ob.name[0]='\0';
			ob.address[0]='\0';
		}
	} null;

	class list
	{
		public:
			list() { last=0; } 
			
			static void init() {}
			
			// Функция вставки объекта в массив
			void INSERT(object& x, pos p); 
			
			// Возвращает позицию 1го элемента в списке или 1го свободного, если он пуст
			pos FIRST() const { return 0; } 
			
			// Возвращается позиция, предшествующая р
			pos PREVIOUS (pos p) const; 
			
			// Возвращается позиция, следующая за р
			pos NEXT(pos p) const; 
			
			// Функция, возвращающая первый свободный элемент
			pos END() const { return last; } 
			
			// Функция, возвращающая позицию объекта
			pos LOCATE(object& x) const; 
			
			// Функция, возвращающая объект в позиции р
			object& RETRIEVE(pos p); 
			
			// Вывод списка на печать
			void PRINTLIST() const; 
			
			// Удаление объекта в позиции р из списка
			pos DELETE(pos p); 
			
			// Функция, которая делает список пустым
			void MAKENULL() { last=0; }; 
			
		private:
			static const int SIZE=10;
			
			object ob[SIZE];
			
			// Позиция первого свободного эл-та
			pos last; 
			
			// Функция для контроля границ массива
			int check_pos(pos p) const; 
			
			// Определяет конец итерации в циклах: возвращает либо первый свободный, либо SIZE, если список полон
			pos check_last() const; 
	};
}

// Функция вставки объекта в массив
void array_based_list::list::INSERT(object& x, pos p) 
{ 
	// Контролируем, чтобы вставка происходила в середину или конец списка
	if (check_pos(p) || p==last) 
	{
		// Начиная с конца, сдвигаем всe объекты поочерёдно до индекса p
		for (pos i=last; i>p; i--)
			ob[i]=ob[i-1]; 
		ob[p]=x;			
	}
	// Если после вставки место ещё есть, увеличиваем индекс last, иначе ставим его на -2
	last!=SIZE-1 ? last++ : last=-2; 
}

// Возвращается позиция, предшествующая р
array_based_list::pos array_based_list::list::PREVIOUS (pos p) const 
{
	// Если массив полностью заполнен, возвращаем последний его элемент
	if (last==-2) 
		return SIZE-1;
	// При передаче несуществующей позиции или при попытке определить эл-т, стоящий перед 1ым, возвращаем -2
	if ((!check_pos(p) && p!=last) || p==0)
		return -2; 
	// В противном случае возвращаем элемент с индексом р-1	
	return p-1; 
}

// Возвращается позиция, следующая за р
array_based_list::pos array_based_list::list::NEXT(pos p) const 
{
	// Если переданная позиция выходит за границы массива, значит, следующего эл-та нет
	if (!check_pos(p) || p==SIZE-1)
		return -2; 
	return p+1;
}

// Функция, возвращающая позицию объекта
array_based_list::pos array_based_list::list::LOCATE(object& x) const 
{
	// Сравниваем каждый объект с переданным объектом
	for (pos i=0; i<last; i++)
	{
		if (ob[i]==x)
			return i; 
	}
	// Если такого объекта нет, возвращаем last
	return last; 
}

// Функция, возвращающая объект в позиции р
object& array_based_list::list::RETRIEVE(pos p) 
{
	// Если позиция выходит за границы массива, возвращаем фиктивный объект
	if (!check_pos(p))
		return null.ob; 
	return ob[p]; 
}

// Удаление объекта в позиции р из списка
array_based_list::pos array_based_list::list::DELETE(pos p) 
{
	// Если позиция не выходит за границы списка
	if (check_pos(p)) 
	{
		pos end=check_last();
		// Сдвигаем все объекты, стоящие после удалённого элемента
		for (pos i=p; i<end; i++) 
			ob[i]=ob[i+1];
		// Первый свободный элемент теперь находится в ячейке с меньшим индексом
		last==-2? last=SIZE-1: last--; 
	}
	return p;
}

// Вывод списка на печать
void array_based_list::list::PRINTLIST() const 
{
	pos end=check_last();
	for (pos i=0; i<end; i++)
		cout<<ob[i].name<<" "<<ob[i].address<<endl;
	cout<<endl;
}

// Функция для контроля границ массива
int array_based_list::list::check_pos(pos p) const 
{
	pos end=check_last();
	return (p>=0 && p<end);
}

// Определяет конец итерации в циклах: возвращает либо первый свободный, либо SIZE, если список полон
array_based_list::pos array_based_list::list::check_last() const 
{
	return last==-2? SIZE: last;
}


/* Реализация списка на односвязном списке */

namespace singly_linked_list
{
	// Элемент списка
	struct elem 
	{
		object ob;
		elem* next; 
		
		elem() { next=0; }
		
		// Инициализция пустых строк
		void init() 
		{
			ob.name[0]='\0';
			ob.address[0]='\0';
			next=this;
		}
	} null;
	
	typedef elem* pos; 
	
	class list
	{
	public:
		list() { head=0; } 
		
		static void init() {}
		
		~list() { MAKENULL(); }
		
		// Функция вставки объекта
		void INSERT(object& ob, pos p); 
		
		// Возвращает позицию 1го элемента в списке
		pos FIRST() const { return head; } 
		
		// Возвращается позиция, предшествующая р
		pos PREVIOUS (pos p) const; 
		
		// Возвращается позиция, следующая за р
		pos NEXT(pos p) const; 
		
		 // Функция, возвращающая первый свободный элемент
		pos END() const { return 0; }
		
		// Функция, возвращающая позицию объекта
		pos LOCATE(object& ob) const; 
		
		// Функция, возвращающая объект в позиции р
		object& RETRIEVE(pos p) const; 
		
		// Вывод списка на печать
		void PRINTLIST() const; 
		
		// Удаление объекта в позиции р из списка
		pos DELETE(pos p); 
		
		// Функция, которая делает список пустым
		void MAKENULL(); 
		
	private:
		pos head;
		
		// Функция для проверки существования переданной позиции (возвращает предыдущую)
		pos check_pos(pos p) const; 
		
		// Функция поиска последнего эл-та
		pos find_last() const; 
	};
}

// Функция вставки объекта в список
void singly_linked_list::list::INSERT(object& ob, pos p) 
{
	// Создаётся новый объект
	pos n=new elem; 
	// Вставка первого элемента в пустой список:
	if (head==0) 
	{
		// В новый объект копируются данные переданного объекта и назначается голова
		n->ob=ob; 
		head=n; 
		return;
	}
	// Вставка в конец:
	if (p==0) 
	{
		n->ob=ob;
		pos temp=find_last();
		temp->next=n;
		return;
	}
	// При вставке в определённую позицию
	if (check_pos(p) || p==head) 
	{
		// В новый объект копируется содержимое элемента в позиции р
		n->ob=p->ob; 
		n->next=p->next;
		p->ob=ob; 
		p->next=n;
	}
}

// Возвращается позиция, предшествующая р
singly_linked_list::pos singly_linked_list::list::PREVIOUS (pos p) const 
{
	// Для головы предыдущего элемента не существует, возвращаем адрес фиктивного объекта
	if (p==head) 
		return &null;
	return check_pos(p);
}

// Возвращается позиция, следующая за р
singly_linked_list::pos singly_linked_list::list::NEXT(pos p) const 
{
	if (p==head) 
		return head->next;
	// Если позиция существует, возвращаем следующий за ней элемент
	if (check_pos(p)) 
		return p->next; 
	return &null;
}

// Функция, возвращающая позицию объекта
singly_linked_list::pos singly_linked_list::list::LOCATE(object& ob) const 
{
	pos temp=head;
	// Сравниваем все объекты с переданным, пока не дойдём до конца
	while (temp!=0) 
	{
		if (temp->ob==ob)
			return temp;
		temp=temp->next;
	}
	return &null;
}

// Функция, возвращающая объект в позиции р
object& singly_linked_list::list::RETRIEVE(pos p) const 
{
	if (p==head)	
		return p->ob;
	if (check_pos(p))
		return p->ob;
	// Если p=END() или такой позиции нет, возвращаем фиктивный объект
	return null.ob; 
}

// Вывод списка на печать
void singly_linked_list::list::PRINTLIST() const 
{
	pos temp=head;
	while (temp!=0) 
	{
		cout<<temp->ob.name<<" "<<temp->ob.address<<endl;
		temp=temp->next;
	}
	cout<<endl;
}

// Удаление объекта в позиции р из списка
singly_linked_list::pos singly_linked_list::list::DELETE(pos p) 
{
	// Удаление головы
	if (p==head) 
	{
		// Если в списке был только 1 элемент, обнуляем значение головы списка
		if (p->next==0)
			head=0; 
		// Иначе переставляем голову на следующий эл-т
		else
			head=head->next; 
		delete p;
		return head;
	}
	// Удаление из определённой позиции
	if (pos prev=check_pos(p)) 
	{
		// Связываем предшествующий элемент со следующим
		prev->next=p->next; 
		pos next=p->next;
		delete p;
		return next;
	}
	// Если переданной позиции не существует, её же и возвращаем
	return p; 
}

// Функция, которая делает список пустым
void singly_linked_list::list::MAKENULL() 
{
	pos temp=head;  
	while (temp!=0)
	{	
		// Запоминаем следующий элемент, удаляем текущий, переходим к следующему
		pos next=temp->next; 
		delete temp; 
		temp=next; 
	}
	// Обнуляем указатель на голову списка
	head=0; 
}

// Функция для проверки существования переданной позиции, возвращает предыдущую
singly_linked_list::pos singly_linked_list::list::check_pos(pos p) const 
{
	// Переменная, хранящая позицию предыдущего элемента
	pos prev=head; 
	// Начинаем поиск со второй позиции
	pos temp=prev->next; 
	// Ищем в списке позицию, переданную в кач-ве входного параметра
	while(temp!=0) 
	{
		if (temp==p)
			return prev;
		prev=temp;
		temp=temp->next;
	}
	// Возвращаем 0, если не нашли позицию в списке
	return 0; 
}

// Функция поиска последнего эл-та
singly_linked_list::pos singly_linked_list::list::find_last() const 
{
	pos prev=head;
	pos temp=prev->next;
	while(temp!=0)
	{
		prev=prev->next;
		temp=temp->next;
	}
	return prev;
}


/* Реализация списка на двусвязном списке */

namespace doubly_linked_list
{
	// Элемент списка
	struct elem 
	{
		object ob;
		elem* prev;
		elem* next; 
		
		elem() { next=0; prev=0; }
		
		// Инициализция пустых строк
		void init() 
		{
			ob.name[0]='\0';
			ob.address[0]='\0';
			next=this;
			prev=this;
		}
	} null;
	
	typedef elem* pos;
	
	class list
	{
	public:
		// Конструктор
		list() { head=0; tail=0; } 
		
		static void init() {}
		
		// Деструктор
		~list() { MAKENULL(); } 
		
		// Функция вставки объекта в заданную позицию
		void INSERT(object& ob, pos p); 
		
		// Возвращает позицию 1го элемента в списке
		pos FIRST() const { return head; } 
		
		// Возвращается позиция, предшествующая р
		pos PREVIOUS(pos p) const; 
		
		// Возвращается позиция, следующая за р
		pos NEXT(pos p) const; 
		
		// Функция, возвращающая первый свободный элемент
		pos END() const { return 0; } 
		
		// Функция, возвращающая позицию объекта
		pos LOCATE(object& ob) const; 
		
		// Функция, возвращающая объект в позиции р
		object& RETRIEVE(pos p) const; 
		
		// Вывод списка на печать
		void PRINTLIST() const; 
		
		// Удаление объекта в позиции р из списка
		pos DELETE(pos p); 
		
		// Функция, которая делает список пустым
		void MAKENULL(); 
		
	private:
		pos head, tail;
		
		// Функция для проверки существования переданной позиции
		int check_pos(pos p) const; 
	};
}

// Функция вставки объекта в заданную позицию
void doubly_linked_list::list::INSERT(object& ob, pos p) 
{
	pos n=new elem;
	// Вставка в пустой список
	if (head==0) 
	{
		// Копируем содержимое, назначаем голову и хвост
		n->ob=ob; 
		head=tail=n; 
		return;
	}
	// Вставка в позицию
	if (check_pos(p)) 
	{
		// В новый элемент копируется содержимое элемента в позиции p
		n->ob=p->ob; 
		// Связываем смещённый элемент со следующим
		n->next=p->next;
		n->next->prev=n;
		// Копируем данные в элемент в позиции р
		p->ob=ob; 
		// Связываем элемент в позиции р со смещённым элементом
		p->next=n; 
		n->prev=p;
		// Если вставили в хвост, изменяем его значение
		if (p==tail) 
			tail=n;
		return;
	}
	// Вставка в конец 
	if (p==0) 
	{
		// Копируем содержимое
		n->ob=ob; 
		// Предыдущим элементом будет хвост
		n->prev=tail; 
		tail->next=n; 
		tail=n;
	}
}

// Возвращается позиция, предшествующая р
doubly_linked_list::pos doubly_linked_list::list::PREVIOUS(pos p) const 
{
	// Если такая позиция существует, и это не голова, то возвращаем предыдущий элемент
	if (check_pos(p) && p!=head) 
		return p->prev;
	if (p==0)
		return tail; 
	return &null;
}

// Возвращается позиция, следующая за р
doubly_linked_list::pos doubly_linked_list::list::NEXT(pos p) const 
{
	// Если такая позиция существует, возвращаем следующий элемент
	if (check_pos(p)) 
		return p->next;
	return &null;
}

// Функция, возвращающая позицию объекта
doubly_linked_list::pos doubly_linked_list::list::LOCATE(object& ob) const 
{
	pos temp=head;
	// Сравниваем все объекты с переданным, пока не дойдём до конца
	while (temp!=0) 
	{
		if (temp->ob==ob)
			return temp;
		temp=temp->next;
	}
	return &null;
}

// Функция, возвращающая объект в позиции р
object& doubly_linked_list::list::RETRIEVE(pos p) const 
{
	// Если такая позиция нашлась, возвращаем объект в этой позиции
	if (check_pos(p)) 
		return p->ob; 
	// Если p=END() или такой позиции нет, возвращаем фиктивный объект
	return null.ob; 
}

// Вывод списка на печать
void doubly_linked_list::list::PRINTLIST() const 
{
	pos temp=head;
	while (temp!=0)
	{
		cout<<temp->ob.name<<" "<<temp->ob.address<<endl;
		temp=temp->next;
	}
	cout<<endl;
}

// Удаление объекта в позиции р из списка
doubly_linked_list::pos doubly_linked_list::list::DELETE(pos p) 
{
	pos next;
	// Удаление головы 
	if (p==head) 
	{
		// Если в списке один элемент, обнуляем значения головы и хвоста
		if (p->next==0) 
		{
			head=tail=0;
			delete p;
			return 0;
		}
		head=head->next;
		head->prev=0;
		delete p;
		return head;	
	}
	// Удаление хвоста
	if (p==tail) 
	{
		tail=tail->prev;
		tail->next=0;
		delete p;
		return 0;
	}
	// Удаление из заданной позиции
	if (check_pos(p)) 
	{
		// Меняем предыдущему элементу ссылку на следующий перед удалением
		p->prev->next=p->next; 
		// Меняем следующему элементу ссылку на предыдущий перед удалением
		p->next->prev=p->prev; 
		next=p->next;
		delete p;
		return next;
	}
	return p;
}

// Функция, которая делает список пустым
void doubly_linked_list::list::MAKENULL() 
{
	pos temp=head;
	while (temp!=0)
	{
		// Запоминаем следующий элемент
		pos next=temp->next; 
		// Удаляем текущий элемент
		delete temp; 
		temp=next;
	}
	// Обнуляем указатель на голову и хвост списка
	head=0; 
	tail=0;
}

// Функция для проверки существования переданной позиции
int doubly_linked_list::list::check_pos(pos p) const 
{
	pos temp=head;
	while (temp!=0)
	{
		// Если соответствующий элемент нашёлся, возвращаем 1
		if (temp==p) 
			return 1;
		temp=temp->next;
	}
	return 0;
}


/* Реализация списка на курсорах */

namespace cursor_linked_list
{
	typedef int pos;
	
	// Элемент списка
	struct elem 
	{
		object ob;
		pos next;
		
		elem() { next=-1; }
		
		// Инициализция пустых строк
		void init() 
		{
			ob.name[0]='\0';
			ob.address[0]='\0';
			next=-1;
		}
	} null;
	
	class list
	{
	public:
		list() { head=-1; }
		
		// Инициализация массива
		static void init(); 
		
		~list() { MAKENULL(); }
		
		void INSERT(object& x, pos p);
		
		// Возвращает позицию 1го элемента в списке
		pos FIRST() const { return head; } 
		
		// Возвращается позиция, предшествующая р
		pos PREVIOUS (pos p) const; 
		
		// Возвращается позиция, следующая за р
		pos NEXT(pos p) const; 
		
		pos END() const { return SPACE; }
		
		// Функция, возвращающая позицию объекта
		pos LOCATE(object& ob) const; 
		
		// Функция, возвращающая объект в позиции р
		object& RETRIEVE(pos p) const; 
		
		void PRINTLIST() const;
		
		// Удаление объекта в позиции р из списка
		pos DELETE(pos p); 
		
		// Функция, которая делает список пустым
		void MAKENULL(); 
		
	private:
		static const int SIZE=10;
		
		// Статический массив элементов
		static elem arr[SIZE]; 
		
		// Голова списка занятых элементов
		pos head; 
		
		// Первый свободный элемент массива
		static pos SPACE; 
		
		// Функция для перемещения элемента из одного списка в другой
		void move(pos& to, pos& from); 
		
		// Функция, проверяющая, есть ли переданная позиция в списке
		pos check_pos(pos p) const; 
		
		// Поиск последнего элемента
		pos find_last() const; 
	};
}

// Инициализация массива
void cursor_linked_list::list::init() 
{
	// Первый свободный элемент
	SPACE=0; 
	// Связываем объекты через поле next
	for (int i=0; i<SIZE-1; i++)
		arr[i].next=i+1; 
	// Последний элемент ссылается на -1
	arr[SIZE-1].next=-1; 
}

// Вставка объекта х в позицию р
void cursor_linked_list::list::INSERT(object& x, pos p) 
{
	// Вставка в пустой список
	if (head==-1) 
	{
		// Переместили голову из списка пустых в список занятых
		move(head, SPACE); 
		// Голова - единственный и последний элемент, поэтому она ссылается на SPACE
		arr[head].next=SPACE; 
		// В заданную позицию вставили объект
		arr[head].ob=x; 
		return;
	}
	// Вставка в конец (после последнего)
	if (p==SPACE) 
	{
		// Нашли последний элемент списка занятых
		pos last=find_last(); 
		// Перед тем, как изменить его значение в функции move, делаем его копию
		pos temp=last; 
		// Переместили будущий элемент в список занятых
		move(last, SPACE);  
		// Записали переданный объект
		arr[last].ob=x; 
		// Предпоследний элемент ссылается на только что добавленный
		arr[temp].next=last; 
		// Последний элемент ссылается на SPACE
		arr[last].next=SPACE; 
		return;
	}
	// При вставке в заданную позицию
	if (check_pos(p)!=-1 || p==head) 
	{
		// Позиция, следующая за той, в которую производится вставка
		pos temp=arr[p].next; 
		// Копируем позицию, в которую производится вставка перед тем, как изменить её 
		pos p_copy=p; 
		// Освобождаем позицию р, перемещая элемент, стоящий в этой позиции, в первую свободную ячейку
		move(p, SPACE); 
		// Эл-т, стоящий в позиции, в которую производится вставка, ссылается на добавленную ячейку
		arr[p_copy].next=p; 
		// Копируем данные в смещённый объект
		arr[p].ob=arr[p_copy].ob; 
		// Записали переданный объект в нужную позицию
		arr[p_copy].ob=x; 
		// Смещённому объекту указываем ссылку на следующий
		arr[p].next=temp; 
	}
}

// Возвращается позиция, предшествующая р
cursor_linked_list::pos cursor_linked_list::list::PREVIOUS (pos p) const 
{
	// Для головы предыдущего элемента не существует, возвращаем -1
	if (p==head) 
		return -1;
	return check_pos(p);
}

// Возвращается позиция, следующая за р
cursor_linked_list::pos cursor_linked_list::list::NEXT(pos p) const 
{
	if (p==head)
		return arr[head].next;
	if (p==find_last())
		return SPACE;
	// Если переданная позиция существует, возвращаем следующую 
	if (check_pos(p)!=-1) 
		return arr[p].next;
	// Если вышли за границы, возвращаем -1
	return -1; 
}

// Функция, возвращающая позицию объекта
cursor_linked_list::pos cursor_linked_list::list::LOCATE(object& ob) const 
{
	pos temp=head;
	// Сравниваем все объекты с переданным, пока не дойдём до конца
	while (temp!=SPACE) 
	{
		if (arr[temp].ob==ob)
			return temp;
		temp=arr[temp].next;
	}
	return -1;
}

// Функция, возвращающая объект в позиции р
object& cursor_linked_list::list::RETRIEVE(pos p) const 
{
	// Если объект с такой позицией существует, возвращаем его
	if ((p==head || check_pos(p)!=-1)) 
		return arr[p].ob;
	// Если p=END() или такой позиции нет, возвращаем фиктивный объект
	return null.ob; 
}

// Вывод списка на печать
void cursor_linked_list::list::PRINTLIST() const 
{
	pos temp=head;
	while (temp!=SPACE) 
	{
		cout<<arr[temp].ob.name<<" "<<arr[temp].ob.address<<endl;
		temp=arr[temp].next;
	}
	cout<<endl;
}

// Удаление объекта в позиции р из списка
cursor_linked_list::pos cursor_linked_list::list::DELETE(pos p) 
{
	// Удаление из головы
	if (p==head) 
	{
		// Если в списке остался только один элемент
		if (arr[head].next==SPACE) 
		{
			SPACE=head;
			head=-1;
			return SPACE;
		}
		// Запомнили старое значение SPACE перед тем, как его изменить
		pos temp=SPACE; 
		// Переместили голову в список пустых элементов
		move(SPACE, head); 
		// Новый space ссылается на старое значние
		arr[SPACE].next=temp; 
		// Возвращаем изменённое значение головы
		return head; 
	}
	pos prev=check_pos(p);
	// Удаление из определённой позиции
	if (prev!=-1) 
	{
		// Запомнили значение space
		pos temp=SPACE; 
		// Запомнили положение удаляемого элемента
		pos ps=p; 
		// Переместили удаляемый элемент в список пустых
		move(SPACE, p); 
		// Предыдущий элемент ссылается на следующий после удалённого
		arr[prev].next=p; 
		// Новый space ссылается на старое значение
		arr[SPACE].next=temp; 
		pos t=check_pos(temp);
		// Последний элемент ссылается на изменённое значение space
		arr[t].next=SPACE; 
		return p;
	}
	// В случае, если не выполнилось ни одно из условий, просто возвращаем переданную позицию
	return p; 
}

// Функция, которая делает список пустым
void cursor_linked_list::list::MAKENULL() 
{
	 // Если список пуст, сразу выходим из функции
	if (head==-1)
		return;
	// Ставим space на голову списка
	SPACE=head; 
	// Голова указывает на -1, значения сброшены
	head=-1; 
}

cursor_linked_list::pos cursor_linked_list::list::SPACE;
cursor_linked_list::elem cursor_linked_list::list::arr[SIZE];

// Функция для перемещения элемента из одного списка в другой
void cursor_linked_list::list::move(pos& to, pos& from) 
{
	pos temp=from;
	from=arr[from].next;
	to=temp;
}

// Функция, проверяющая, есть ли переданная позиция в списке
cursor_linked_list::pos cursor_linked_list::list::check_pos(pos p) const 
{
	pos prev=head;
	pos temp=arr[prev].next;
	// Ищем переданную позицию в списке
	while (temp!=SPACE) 
	{
		if (p==temp)
			return prev;
		prev=temp;
		temp=arr[temp].next;
	}
	if (p==SPACE)
		return prev;
	return -1;
}

// Поиск последнего элемента
cursor_linked_list::pos cursor_linked_list::list::find_last() const 
{
	pos prev=head;
	pos temp=arr[prev].next;
	// Просматриваем все эл-ты, пока не дойдём до того, у которого не существует следующего
	while (temp!=SPACE) 
	{
		temp=arr[temp].next;
		prev=arr[prev].next;
	}
	return prev;
}


//using namespace array_based_list;
//using namespace singly_linked_list;
using namespace doubly_linked_list;
//using namespace cursor_linked_list;

