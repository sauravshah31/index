all: 
	g++ -g main.cpp utils.cpp insert.cpp create.cpp select.cpp callback.cpp index.cpp btree.cpp -l sqlite3