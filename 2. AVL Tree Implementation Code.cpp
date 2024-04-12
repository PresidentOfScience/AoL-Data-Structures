#include <stdio.h>
#include <stdlib.h>

struct AVL{
	int key;
	int height;
	struct AVL* right;
	struct AVL* left;
};

//fungsi untuk membuat node baru
struct AVL* createNode(int key){
	struct AVL* newNode = (struct AVL*)malloc(sizeof(struct AVL));
	newNode->key = key;
	newNode->height = 1;
	newNode->right = NULL;
	newNode->left = NULL;
	return newNode; //mereturn alamat dari node yang telah dibuat
}

//fungsi untuk mengetahui angka maksimum dari dua angka yang diberikan
int max(int a, int b){
	if(a > b) {
		return a;
	} else {
		return b;
	}
}

//fungsi untuk mendapatkan ketinggian dari suatu node dalam tree
int getHeight(struct AVL* curr){
	if(curr == NULL) {
		return 0;
	} else {
		return curr->height;
	}
}

//fungsi untuk mendapatkan balance factor (yaitu tinggi subtree kiri dikurangi tinggi subtree kanan)
int getBalanceFactor(struct AVL* curr){
	if (curr == NULL) {
		return NULL;
	} else {
		return (getHeight(curr->left) - getHeight(curr->right));
	}
}

//fungsi untuk melakukan left rotation
struct AVL* leftRotate(struct AVL* curr){
	struct AVL* child = curr->right; //menyimpan anak kanan dari curr ke child
	struct AVL* subChild = child->left; //menyimpan anak kiri child ke subChild
	
	child->left = curr; //menjadikan curr sebagai anak kiri dari child, kini child menjadi parent dari curr
	curr->right = subChild; //dan subChild sebagai anak kanan dari curr
	
	//mengupdate ketinggian dari curr dan child
	curr->height = 1 + max(getHeight(curr->left), getHeight(curr->right));				
	child->height = 1 + max(getHeight(child->left), getHeight(child->right));
	
//	mengupdate curr menjadi child dengan return child
	return child;
}

//fungsi untuk melakukan right rotation
struct AVL* rightRotate(struct AVL* curr){
	struct AVL* child = curr->left; //menyimpan anak kiri dari curr ke child
	struct AVL* subChild = child->right; //menyimpan anak kanan dari child ke subChild
	
	child->right = curr; //menjadikan curr sebagai anak kanan dari child, child menjadi parent dari curr
	curr->left = subChild;// subchild menjadi anak kiri dari curr
	
	//mengupdate ketinggian dari curr dan child
	curr->height = 1 + max(getHeight(curr->left), getHeight(curr->right));				
	child->height = 1 + max(getHeight(child->left), getHeight(child->right));
	
	//mereturn child untuk mengupdate curr menjadi child
	return child;
}

//fungsi untuk menyeimbangkan pohon AVL
struct AVL* balance(struct AVL* curr){
	//mengupdate ketinggian curr terlebih dahulu
	curr->height = max(getHeight(curr->right), getHeight(curr->left)) + 1;
	int balanceFactor = getBalanceFactor(curr); //menyimpan balance factor dari curr
	
	//jika balance factor curr lebih besar dari pada 1 dan balance factor anak kiri dari curr lebih besar sama dengan dari 0, maka ini merupakan left left case, sehingga dibutuhkan right rotation untuk menyeimbangkannya
	if (balanceFactor > 1 && getBalanceFactor(curr->left) >= 0) {
		return rightRotate(curr);
	} 
	//jika balance factor curr lebih besar dari pada 1 dan balance factor anak kiri dari curr lebih kecil dari 0, maka ini merupakan left right case, dimana dibutuhkan double rotation
	else if (balanceFactor > 1 && getBalanceFactor(curr->left) < 0) {
		curr->left = leftRotate(curr->left); //lakukan left rotation dulu untuk mengubahnya menjadi left left case, lalu lakukan right rotation untuk menyeimbangkannya
		return rightRotate(curr);
	} 
	//jika balance factor curr lebih kecil dari -1 dan balance factor anak kanan dari curr lebih kecil sama dengan dari 0, maka ini adalah right right case, jadi cukup melakukan rotateleft sekali untuk menyeimbangkannya
	else if (balanceFactor < -1 && getBalanceFactor(curr->right) <= 0) {
		return leftRotate(curr);
	}
	//jika balance factor curr lebih kecil dari -1 dan balance factor anak kanan dari curr lebih besar dari 0, maka ini adalah right left case, diperlukan double rotation untuk menyeimbangkannya
	else if (balanceFactor < -1 && getBalanceFactor(curr->right) > 0) {
		curr->right = rightRotate(curr->right); //pertama dilakukan right rotation dan dilanjuti dengan left rotation untuk menyeimbangkan tree
		return leftRotate(curr);
	}
	//mereturn curr untuk mengupdate pointer yang harus menunjuk ke curr
	return curr;
}

//fungsi ini menginsert node ke posisi sama seperti pada BST, namun akan selalu dibalance treenya supaya tidak mengandung violation
struct AVL* insert(struct AVL* curr, struct AVL* newNode){
	if(curr == NULL){
		return newNode; 
	}else if(newNode->key > curr->key){
		curr->right = insert(curr->right, newNode);
	}else if(newNode->key < curr->key){
		curr->left = insert(curr->left, newNode);
	}else{
		return curr;
	}
	return balance(curr); //balancing terjadi setiap kali direturn
}

//fungsi untuk mendapatkan predecessor (value yang nilainya paling mendekati dan bernilai lebih kecil dari node)
struct AVL* getPredecessor(struct AVL* curr) {
	struct AVL* predecessor = curr->left; //traversal aka dimulai dari anak kiri dari current node
	while (predecessor->right) { //melakukan traversal ke anak kanan hingga sampai leaf)
		predecessor = predecessor->right;
	}
	return predecessor; //mengembalikan predecessor yang sudah ditemukan
}

//fungsi untuk menghapus node
struct AVL* deleteNode(int key, struct AVL* curr){
	//delete akan melakukan traversal yang sama seperti deletion pada BST, namun bedanya adalah tree akan selalu dibalance pada setiap traversal
	if (curr == NULL) {
		return NULL;
	} else if (key < curr->key) {
		curr->left = deleteNode(key, curr->left);
	} else if (key > curr->key) {
		curr->right = deleteNode(key, curr->right);
	} else {
		//terdapat 3 kasus jika sudah ketemu node yang akan dihapus
		//pertama, node tersebut tidak memiliki anak, maka dapat langsung dihapus
		if (curr->left == NULL && curr->right == NULL) {
			free(curr);
			curr = NULL;
		} 
		//kedua, ketika node memiliki 1 anak (kiri atau kanan), sehingga harus memindahkan posisi anak dari curr ke posisi curr
		else if (curr->right == NULL || curr->left == NULL) {
			struct AVL* childNode = (curr->right) ? curr->right : curr->left;
			free(curr);
			curr = childNode;
		} 
		//ketiga, ketika node memiliki 2 anak (kiri dan kanan), sehingga harus menggantikan node saat ini dengan predecessor
		else {
			struct AVL* predecessor = getPredecessor(curr);
			curr->key = predecessor->key;
			curr->left = deleteNode(predecessor->key, curr->left);
		}
	}
	
	if(curr == NULL) {
		return NULL; //jika node saat ini NULL maka tidak perlu melakukan proses balance dan dapat langsung di return
	}
	return balance(curr); //dan akan dilakukan kembali proses balancing
}

//fungsi untuk print tree secara in order
void inOrder(struct AVL* curr){
	if(curr != NULL){
		inOrder(curr->left);
		printf("%d ", curr->key);
		inOrder(curr->right);
	}
}

//fungsi untuk print tree secara pre order
void preOrder(struct AVL* curr){
	if(curr != NULL){
		printf("%d ", curr->key);
		preOrder(curr->left);
		preOrder(curr->right);
	}
}

//fungsi untuk print tree secara post order
void postOrder(struct AVL* curr){
	if(curr != NULL){
		postOrder(curr->left);
		postOrder(curr->right);
		printf("%d ", curr->key);
	}
}

//fungsi search digunakan untuk mencari key, jika key ada, maka fungsi akan mereturn integer 1, jika tidak maka akan mereturn integer 0
int search(struct AVL* curr, int keyToFind) {
	if (curr == NULL) {
		return 0;
	} else if (curr->key > keyToFind) {
		return search(curr->left, keyToFind);
	} else if (curr->key < keyToFind) {
		return search(curr->right, keyToFind);
	}
	return 1;
}

int main(){
	struct AVL* root = NULL;
	int option = 0, num = 0;
	do {
		printf("1. Insertion\n");
		printf("2. Deletion\n");
		printf("3. Traversal\n");
		printf("4. Exit\n");
		printf("Choose: ");
		scanf("%d", &option);
		getchar();
		
		switch(option) {
			case 1:
				//opsi 1 jika ingin menginsert node baru
				printf("Insert: ");
				scanf("%d", &num);//mengisi value dari node baru
				getchar();
				root = insert(root, createNode(num)); //kemudian dibuatkan node baru berisi value dari input user
				
				break;
			case 2:
				//opsi 2 jika ingin menghapus node
				printf("Delete: ");
				scanf("%d", &num); //input node yang ingin dihapus
				getchar();
				if (search(root, num) == 1) {
					//jika node yang ingin dihapus ada
					printf("Data Found\n");
					root = deleteNode(num, root);
					printf("Value %d was deleted\n", num);
				} else {
					//jika data tidak ditemukan, maka tidak perlu menghapus node apapun
					printf("Data not found\n");
				}
				
				break;
			case 3:
				printf("Preorder: ");
				preOrder(root);
				printf("\n");
				
				printf("Inorder: ");
				inOrder(root);
				printf("\n");
				
				printf("Postorder: ");
				postOrder(root);
				printf("\n");
				
				break;
			default:
				break;		
		}
		printf("\n");
	} while (option != 4); // do-while loop akan berhenti jika opsi input adalah 4 
	printf("Thank you\n\n");
	return 0;
}
