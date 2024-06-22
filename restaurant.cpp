#include "main.h"

int MAXSIZE;
vector <vector<int>> DD(505, vector<int>(500 + 5, 0));

int binaryStringToInt(const std::string& binaryString) {
	int decimalNumber = 0;
	int binaryDigit;

	for (char bit : binaryString) {
		binaryDigit = bit - '0';  
		decimalNumber = (decimalNumber << 1) | binaryDigit;
	}

	return decimalNumber;
}
class character_LIST{
	
	public:
		map <char, int> mp;
	public:
		character_LIST(string name)	{
			for(int i = 0; i < (int)name.length(); i++)	{
				mp[name[i]] ++;
			}
		}
		bool checker()			{ return (mp.size() >= 3);}
		bool ceasar_Encode()	{
			map <char, int> encode_MAP;
			
			for(auto i : mp) 	{
				char T;
				
				if (i.first >= 'A' && i.first <= 'Z') 		{	T = 'A' + (i.first - 'A' + i.second) % 26; } 
				else if (i.first >= 'a' && i.first <= 'z') 	{	T = 'a' + (i.first - 'a' + i.second) % 26; }
				else	return false; /// ma hoa that bai
				
				encode_MAP[T] += i.second;
			}
			mp.clear();
			mp 	= encode_MAP; 
			return true;
		}
		string Encode_name_2_Ceasar(string name)	{
			string s = "";
			for(int i = 0 ; i < (int)name.length(); i++)	{
				char temp = name[i];
				if(temp >= 'A' && temp <= 'Z')				{
					auto k = mp.find(temp);
					temp = 'A' + (temp - 'A' + k->second) % 26;
				}
				else if(temp >= 'a' && temp <= 'z')			{
					auto k = mp.find(temp);
					temp = 'a' + (temp - 'a' + k->second) % 26;
				}
				s += temp;
			}
			return s;
		}
		vector <pair<char, int>> map_2_vector()		{
			vector <pair<char, int>> RES(mp.begin(), mp.end());
			return RES;
		}
		~character_LIST() 			{	mp.clear();}
};

class HUFF_NODE		{
	public:
		char val;
		int wgt;
		HUFF_NODE *lc;
		HUFF_NODE *rc;
		HUFF_NODE(char &val, int &wgt) : val(val), wgt(wgt), lc(nullptr), rc(nullptr) {}  // TAO NODE LA
		HUFF_NODE(HUFF_NODE *&lc, HUFF_NODE *&rc) : lc(lc), rc(rc)	{
			val = '$';
			wgt = lc->wgt + rc->wgt;
		}
		bool isLeaf()	{ return !lc && !rc;}
		~HUFF_NODE ()	{}
};
class HUFFMAN_TREE	{
	public:
		int TIME;
		int DECODE_VAL;
		HUFF_NODE *ROOT;
		HUFFMAN_TREE (char val = '$', int wgt = 0) {
			ROOT = new HUFF_NODE(val, wgt);
		}
		HUFFMAN_TREE (HUFF_NODE *&l, HUFF_NODE *&r)	{ ROOT = new HUFF_NODE(l, r);}
		
		HUFFMAN_TREE  (HUFFMAN_TREE *&A_TREE)		{
			this->TIME 			= 	A_TREE->TIME;
			this->DECODE_VAL 	=	A_TREE->DECODE_VAL;
			// SHALLOWED COPY ROOT
			this->ROOT 			= 	A_TREE->ROOT;
		}
		int  get_Height(HUFF_NODE *&root)	 {
			if(!root) return 0;
			return 1 + max(get_Height(root->lc), get_Height(root->rc));
		}
		HUFF_NODE *Rotate_Right	(HUFF_NODE *node)	{
			HUFF_NODE *rNode = node->lc;
			node->lc 	=	rNode->rc;
			rNode->rc 	=	node;
			return rNode;  
		}
		HUFF_NODE *Rotate_Left	(HUFF_NODE *node)	{
			HUFF_NODE *rNode = node->rc;
			node->rc 	=	rNode->lc;
			rNode->lc 	=	node;
			return rNode;  
		}
		HUFF_NODE *Left_Balance(HUFF_NODE *root)		{
			HUFF_NODE *lNode = root->lc;
			int Lf 	=	get_Height(lNode->lc);
			int Rf	= 	get_Height(lNode->rc);

			if(Lf < Rf)	{	// LR
				root->lc 	= Rotate_Left(lNode);
				root		= Rotate_Right(root);
			}
			else 		{	// LL
				root	= Rotate_Right(root);
			}
			return root;
		}
		HUFF_NODE *Right_Balance(HUFF_NODE *root)		{
			HUFF_NODE *rNode = root->rc;
			int Lf 	=	get_Height(rNode->lc);
			int Rf	= 	get_Height(rNode->rc);

			if(Lf > Rf)	{	// RL
				root->rc 	= Rotate_Right(rNode);
				root		= Rotate_Left(root);
			}
			else 		{	// RR
				root	= Rotate_Left(root);
			}
			return root;
		}
		void BALANCE	(HUFF_NODE *&ROOT, int &COUNT)	{
			if(!ROOT) return;
			int L	 =	get_Height(ROOT->lc);
			int R	 =	get_Height(ROOT->rc);

			if(COUNT >=3 ) return;
			else	{
				if(abs(L - R) <= 1)	{
					BALANCE(ROOT->lc, COUNT);
					BALANCE(ROOT->rc, COUNT);
				}
				else{
					if(L > R)	{
					ROOT = Left_Balance(ROOT); // LL LR
				}
				else ROOT = Right_Balance(ROOT);  // RR RL
				COUNT++;

				BALANCE(ROOT->lc, COUNT);
				BALANCE(ROOT->rc, COUNT);
				BALANCE(ROOT, COUNT); 
				}
			}
		}
		void ENCODE(HUFF_NODE *&root, map <char, string> &mp, string s)	{
			if(!root) return;

			if(root->isLeaf()) {
				mp[root->val]	= s; 
			}
			else {
				ENCODE(root->lc, mp, s + '0');
				ENCODE(root->rc, mp, s + '1');
			}
		}
		map <char, string> ENCODE_LIST()	{
			map <char, string> res;
			if(ROOT->isLeaf())	{ return res;}
			else{
				ENCODE(ROOT->lc, res, "0");
				ENCODE(ROOT->rc, res, "1");
			}
			return res;
		}
	void delete_tree(HUFF_NODE *&root)	{
		if	(root)	{
			delete_tree(root->lc);
			delete_tree(root->rc);
			delete root;
			root 	= nullptr;
		}
	}
	void print(HUFF_NODE *&root)		{
		if(!root) return;

		print(root->lc);

		if(root->isLeaf()) cout << root->val << endl;
		else cout << root->wgt << endl;

		print(root->rc);
	}
	~HUFFMAN_TREE()	{}
};
struct Compare_HUFF {
    bool operator () (const HUFFMAN_TREE *l, const HUFFMAN_TREE *r) const {
        if (l->ROOT->wgt > r->ROOT->wgt) 			return true;
        else if (l->ROOT->wgt == r->ROOT->wgt) 	{
            if (l->ROOT->isLeaf() && r->ROOT->isLeaf()) {
                if (islower(l->ROOT->val) && islower(r->ROOT->val)) return l->ROOT->val > r->ROOT->val;
                if (isupper(l->ROOT->val) && isupper(r->ROOT->val)) return l->ROOT->val > r->ROOT->val;
                else return islower(r->ROOT->val);
            }
            return l->TIME > r->TIME;
        }
        return false;
    }
};


class __GOJO___RESTAURANT{
	public:
		class BST_TREE{
			public:
			class Node{
				public:
					int val;
					Node *lc;
					Node *rc;
					Node(const int &val, Node *lc = nullptr, Node *rc = nullptr) : val(val), lc(lc), rc(rc) {}	
					bool isLeaf()	{ return (!lc && !rc);}		
			};
			queue <int> lst;
			Node *ROOT;
			public:
				BST_TREE () : ROOT(nullptr) {}
				int DEM_SO_PHAN_TU(Node * root)		{
					if(!root) return 0;
					return 1 + (DEM_SO_PHAN_TU(root->lc)) + (DEM_SO_PHAN_TU(root->rc));
				}
				int COUNTING_KOKUSHEN	(Node * root) 	{
					if(!root) return 1;
					int x = DEM_SO_PHAN_TU(root->lc);
					int y = DEM_SO_PHAN_TU(root->rc);
					return (((COUNTING_KOKUSHEN(root->lc) * COUNTING_KOKUSHEN (root->rc)) % MAXSIZE) * DD[x][x+y]) % MAXSIZE;
				}

				void push(Node *& root, const int &val){
					if(!root) { root = new Node(val); return;}

					if(root->val > val) push(root->lc, val);
					else push(root->rc, val);
				}
				void push(int num) {
					push(ROOT, num);
					lst.push(num);
				}
				int size() const { return (int)lst.size();}

				bool DEL(Node *&root,const int &val){
					if(!root) return false; // ko ton tai value

					if(root->val > val) return DEL(root->lc, val);
					else if(root->val < val) return DEL(root->rc, val); 
					else{
						if(root->lc && root->rc) {
							Node *temp = root->rc;
							while(temp->lc) {temp = temp->lc;}

							root->val 	= 	temp->val;
							return DEL(root->rc, root->val);
						}
						if(root->lc) {
							Node *TEMP = root;
							root = root->lc;
							delete TEMP;
						}
						else if(root->rc) {
							Node *TEMP = root;
							root = root->rc;
							delete TEMP;
						}
						else {
							delete root;
							root = nullptr;
						}
					}
					return true;
				}
				void DELETE(int &Y){
					bool Success = true;
					while( Y && lst.size() && Success)	{
						int M = lst.front();
						lst.pop();
						Success = this->DEL(ROOT, M);
						Y--;
					}
				}
				void print(Node *&root)	{
					if(!root) return;
					print(root->lc);
					cout << root->val << endl;
					print(root->rc);
				}
				void print() { print(ROOT);}
				~BST_TREE() {
					while(DEL(ROOT, ROOT->val)) {}
				}
		};
	public:
		vector <BST_TREE *> HASHTABLE;
		__GOJO___RESTAURANT () {
			HASHTABLE.resize(MAXSIZE + 2, nullptr);
		}
		void push(int num)	{
			int ID = num % MAXSIZE + 1;

			if(!HASHTABLE[ID]) HASHTABLE[ID] = new BST_TREE;
			HASHTABLE[ID]->push(num);
		}
		void KOKUSEN_G()	{
			for(int i = 0, n = (int)HASHTABLE.size(); i < n; i++){
				if(!HASHTABLE[i]) continue;
				else{
					int Y 	= HASHTABLE[i]->COUNTING_KOKUSHEN(HASHTABLE[i]->ROOT);
					Y 		= Y % MAXSIZE;
					HASHTABLE[i]->DELETE(Y);

					if(HASHTABLE[i]->size() == 0){
						delete HASHTABLE[i];
						HASHTABLE[i] 		= nullptr;
					}
				}
			}
		}

		void print(int ID)	{
			if(ID < 0 || ID > MAXSIZE || !HASHTABLE[ID]) return;
			HASHTABLE[ID]->print();
		}
		~__GOJO___RESTAURANT()	{
			for(BST_TREE * i : HASHTABLE)	{
				if(i != nullptr) delete i;
			}
		}
};

class Node{
	public:
		int ne;
		int TAG;
		int PROCESSING;
		vector <int> RES_VAL;
		Node () :	ne(0) {}
		void push(const int &res) 	{	ne++; RES_VAL.push_back(res);} 
		void print_LIFO	(const int &num)	{ 
			for(int n = (int)RES_VAL.size(), i = n - 1, t = num; i >= 0 && t > 0; i--, t--)	
			{ cout << TAG << "-" << RES_VAL[i] << endl;}
		}
		void KEITEIKEN_H(const int &num, const int &PRO)	{
			int T = num;
			while	(!RES_VAL.empty() && T > 0)	{
				cout << RES_VAL[0] << "-" << TAG << endl;
				RES_VAL.erase(RES_VAL.begin());
				ne--;
				T--;
			}
			PROCESSING = PRO;
		}
};
class Comp	{
	public:
		static bool prior (const Node &A, const Node &B)	{
			if(A.ne < B.ne) return true;

			return (A.ne == B.ne) ? (A.PROCESSING < B.PROCESSING) : false;
		}
};
class ___HEAP{
	public:
		void swap(vector <Node> &ARR, const int &first, const int &second)    {
            Node TMP    = std::move(ARR[first]);
            ARR[first]  = std::move(ARR[second]);
            ARR[second] = std::move(TMP);
        }
	public:
		vector <Node> heap;
		int maxsize;
		int ne;
		int PROCESS;
	private:
		void reheapDown	(int pos)	{
			while (!this->isLeaf(pos))
			{
				int j = this->leftchild(pos); int rc = this->rightchild(pos);
				if((rc < ne) && Comp::prior(heap[rc], heap[j])) j = rc;

				if(Comp::prior(heap[pos], heap[j])) return;
				
				swap(heap, pos, j);
				pos = j;
			}
		}
	public:
		___HEAP()	{	heap.resize(MAXSIZE + 5); maxsize = MAXSIZE + 5; ne = 0; PROCESS = 0;}							
		
		bool 	isLeaf		(int index)		{ return index >= ne/2 && index < ne;}

		int 	leftchild	(int index) 	{ return 2 * index + 1;}
		int 	rightchild	(int index) 	{ return 2 * index + 2;}

		int 	Parent		(int index)		{ return (index - 1)/2;}
		void 	buildHeap 	()				{ for(int i = ne/2; i >= 0; i--) reheapDown(i);}
		
		void 	insert		(Node &it)	{ 
			int curr 	= 	ne++;
			heap[curr] 	=	std::move(it);
			while((curr != 0) && Comp::prior(heap[curr], heap[Parent(curr)])){
				swap(heap, curr, Parent(curr));
				curr 	= Parent(curr);
			}
		}
		int FIND_TAG		(const int &ID)		{
			for(int i = 0; i < ne; i++)	{
				if(heap[i].TAG == ID) return i;
			}
			return -1;
		}
		void 	insert		(const int & result)		{
			int ID = result % MAXSIZE;

			int T = FIND_TAG(ID + 1);

			if(T != -1)		{
				heap[T].push(result);
				heap[T].PROCESSING = ++PROCESS;
				
				while((T != 0) && Comp::prior(heap[T], heap[Parent(T)])){
					swap(heap, T, Parent(T));
					T 	= Parent(T);
				}
				this->reheapDown(T);
			}
			else			{
				Node I; I.push(result); I.TAG = ID + 1; 
				I.PROCESSING = ++PROCESS;
				this->insert(I);
			}
			
		}
		void DUYET_PREORDER		(int R,const int &num)		{ //root ->left ->right
			if(R < 0 || R >= ne || num <= 0) return;
			heap[R].print_LIFO(num);
			DUYET_PREORDER	(this->leftchild(R), num);
			DUYET_PREORDER	(this->rightchild(R), num);
		}
		void removefirst		()				{
			swap(heap, 0, --ne);
			if(ne != 0) reheapDown(0);
			return;
		}
		void remove			(int pos)		{
			if(pos > ne - 1) return;

			swap(heap, pos, --ne);
			while(pos != 0 && Comp::prior(heap[pos], heap[Parent(pos)])){
				swap(heap, pos, Parent(pos));
				pos 	= 	Parent(pos);
			}
			if(ne != 0)  reheapDown(0);
			return;
		}
		void reheapUp		(int pos)		{
			while(pos != 0 && Comp::prior(heap[pos], heap[Parent(pos)])){
				swap(heap, pos, Parent(pos));
				pos 	= 	Parent(pos);
			}
		}
		void KEITEIKEN_SH	(const int &num){
			vector <Node> __PRIOR(heap.begin(), heap.begin() + ne);
			sort(__PRIOR.begin(), __PRIOR.end(), Comp::prior);
			int COUNT = num;

			for	(auto i : __PRIOR)	{
				for(int j = 0; j < ne; j++)	{
					if(heap[j].TAG == i.TAG)  { 	
						heap[j].KEITEIKEN_H(num, ++PROCESS); 
						if(heap[j].ne == 0) remove(j);
						else	{	reheapUp(j);	}
						COUNT--; 
						break; 
					}
				}
				if(COUNT == 0) return;
			}
		}
		int size 			()				{ return ne;}
		~___HEAP			()				{ heap.clear();}
};
class __SUKUNA_RESTAURANT{
	public:
		___HEAP S;
		__SUKUNA_RESTAURANT()			{}
		void push	(const int &res)	{
			S.insert(res);
		}
		void print	(int &num)					{
			S.DUYET_PREORDER(0, num);
		}

		void KEITEIKEN_S(const int num)	{
			S.KEITEIKEN_SH(num);
		}
		~__SUKUNA_RESTAURANT()			{}
};
HUFFMAN_TREE *BUILD_TREE(string &name)	{
			int time = 0;
			character_LIST ENCODE(name);
			
			string name_encoded = ENCODE.Encode_name_2_Ceasar(name);
			
			if (!ENCODE.checker()  || !ENCODE.ceasar_Encode()) return nullptr;
			
			vector <pair<char, int>> MAP = ENCODE.map_2_vector();

			priority_queue <HUFFMAN_TREE *, vector <HUFFMAN_TREE *>, Compare_HUFF> PQ;

			for(int i = 0, n  = (int)MAP.size(); i < n; i++) {
				HUFFMAN_TREE *TEMP	= new HUFFMAN_TREE(MAP[i].first, MAP[i].second);
				TEMP->TIME 	=	++time; 
				PQ.push(TEMP); 
			}
			while(PQ.size() > 1)	{
				HUFFMAN_TREE	*TEMP_1, *TEMP_2;

				TEMP_1 = PQ.top();   PQ.pop();
				TEMP_2 = PQ.top();   PQ.pop();
			
				HUFFMAN_TREE *TEMP_3	= new HUFFMAN_TREE(TEMP_1->ROOT, TEMP_2->ROOT);
				
				delete TEMP_1; delete TEMP_2;
				int COUNT = 0;
				TEMP_3->BALANCE(TEMP_3->ROOT, COUNT);
				TEMP_3->TIME 	= ++time;
				PQ.push(TEMP_3);
			}
			HUFFMAN_TREE *TMP = PQ.top();
			PQ.pop();
			
			if(TMP->ROOT->isLeaf()) { TMP->DECODE_VAL = 0; return TMP;}
			
			map <char, string> mp;	mp = TMP->ENCODE_LIST();

			string BIN_CODE = "";
			//______________________________________________________________
			for(int i = 0 ; i < (int)name_encoded.length(); i++)	{char T = name_encoded[i]; auto k = mp.find(T); BIN_CODE +=	k->second; }
			int AT = max(0, (int)BIN_CODE.length() - 10);
			string BIN_NUM = BIN_CODE.substr(AT);
			reverse(BIN_NUM.begin(), BIN_NUM.end());
			
			int NUM = binaryStringToInt(BIN_NUM);
			TMP->DECODE_VAL	= NUM; 
			return TMP;
		}
class imp_res{
	public:
		HUFFMAN_TREE *TREE;
		__GOJO___RESTAURANT *G;
		__SUKUNA_RESTAURANT *S;
	public:
		imp_res()									{
			TREE = 	nullptr;
			G 	 =	nullptr;
			S 	 =	nullptr;
		}
		void CHOOSE_RESTAURANT(int Result)			{ 
			if(Result % 2 == 0){
				if(!S) S = new __SUKUNA_RESTAURANT();
				S->push(Result);
			}
			else{
				if(!G) G = new __GOJO___RESTAURANT();
				G->push(Result);
			}
		}
		void LAPSE(string name)						{ 
			HUFFMAN_TREE *A_TREE = BUILD_TREE(name);
			if (!A_TREE) return;

			if(!TREE) 	TREE = A_TREE;
			else{
				TREE->delete_tree(TREE->ROOT);
				delete TREE;
				TREE = A_TREE;
			}
			CHOOSE_RESTAURANT(TREE->DECODE_VAL);
		}
		void KOKUSEN()			{ if(G) G->KOKUSEN_G(); }
		void KEITEIKEN(int num)	{ if(S) S->KEITEIKEN_S(num);}
		void HAND()				{ if(TREE) TREE->print(TREE->ROOT);}
		void LIMITLESS(int num)	{ if(G) G->print(num);}
		void CLEAVE(int num)	{ if(S) S->print(num);}
		~imp_res()				{
			if(TREE) {
				TREE->delete_tree(TREE->ROOT);
				delete TREE;
			}
			if(G)	delete G;
			if(S)	delete S;
			
		}
};


void simulate(string filename)
{
	imp_res *r = new imp_res();
	ifstream ss(filename);
	string str, name, num, maxsize;
	while (ss >> str)
	{
		if(str == "MAXSIZE"){
			ss >> maxsize;
			MAXSIZE = stoi(maxsize);

            for(int i = 0; i < 500; i++) DD[0][i]=1;
			for(int i = 0; i < 500; i++) DD[i][i]=1;
			for(int i = 1; i < 500; i++)		{
				for(int j = i+1; j <500; j++)	{
					DD[i][j] = DD[i][j-1] + DD[i - 1][j - 1];
				}
			}
		}
		else if(str == "LAPSE"){
			ss >> name;
			r->LAPSE(name);
		}
		else if(str == "KOKUSEN"){
			r->KOKUSEN();
		}
		else if(str == "KEITEIKEN"){
			ss >> num;
			r->KEITEIKEN(stoi(num));
		}
		else if(str == "HAND"){
			r->HAND();
		}
		else if(str == "LIMITLESS"){
			ss >> num;
			r->LIMITLESS(stoi(num));
		}
		else{
			ss >> num;
			r->CLEAVE(stoi(num));
		}
	}
	DD.clear();
	delete r;
}