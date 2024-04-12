//	Anggota kelompok:
//	1. 2602092150 - Fendy Wijaya
//	2. 2602093802 - Wilbert Yang
//	3. 2602095852 - Vincent Owen Bun
//	4. 2602100606 - Trinata Suryawan
//	5. 2602125363 - Bernard Owens Wiladjaja
//	6. 2602196519 - David

//			Red-Black Tree

#include <stdio.h>
#include <stdlib.h>

struct RBT {
    int key;
    int color; //merah bervalue 1, hitam bervalue 0

    struct RBT* left;
    struct RBT* right;
    struct RBT* parent;
};

//fungsi untuk membuat node
struct RBT* createNode(int key) {
    struct RBT* newNode = (struct RBT*)malloc(sizeof(struct RBT));
    newNode->key = key;
    newNode->color = 1; //selalu berwarna merah (angka 1) saat pertama kali dibuat
    
    //pointer diset ke NULL semua
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    //mengembalikan alamat dari node yang telah dibuat
    return newNode;
}

//fungsi untuk melakukan left rotation
struct RBT* leftRotation(struct RBT* root, struct RBT* node) {
    struct RBT* rightNode = node->right; //anak kanan dari node saat ini akan disimpan dalam rightNode
    node->right = rightNode->left; // anak kanan dari node akan menyimpan anak kiri dari rightNode
    
	//jika anak kiri dari rightNode tidak kosong, maka akan dihubungkan dengan parentnya
	if (rightNode->left != NULL) {
        rightNode->left->parent = node;
    }
    
    //lalu menghubungkan rightNode dengan parent dari node
    rightNode->parent = node->parent;
    
    if (node->parent == NULL) {
        root = rightNode; //jika jika parent dari node kosong, artinya node sekarang adalah root, maka rightNode yang akan menjadi root baru
    } else if (node == node->parent->left) {
        node->parent->left = rightNode; //jika node saat ini merupakan anak kiri dari parent, maka dapat mengubah anak kiri parent menjadi rightNode
    } else {
        node->parent->right = rightNode;//jika node kini adalah anak kanan dari parent, maka mengubah anak kanan parent menjadi rightNode
    }
    //kondisi saat ini berarti rightNode sekarang merupakan parent dari node, sehingga kita menghubungkan node->parent ke rightNode, dan anak kiri rightNode menjadi node
    rightNode->left = node;
    node->parent = rightNode;
    
    //kemudian mengembalikan kembali alamat root untuk mengupdate rootnya
    return root;
}

//fungsi untuk melakukan right rotation
struct RBT* rightRotation(struct RBT* root, struct RBT* node) {
    struct RBT* leftNode = node->left; //anak kiri dari node disimpan dalam leftNode
    node->left = leftNode->right; //anak kanan dari leftNode akan menjadi anak kiri dari node
    
    //jika anak kanan dari leftNode tidak kosong, anak kanan leftNode akan dihubungkan ke parent baru yaitu node
    if (leftNode->right != NULL) {
    	leftNode->right->parent = node;
    }
    
    //menguhubungkan leftNode dengan parent dari node
    leftNode->parent = node->parent;
    
    if (node->parent == NULL) {
        root = leftNode; //jika parent node kosong, maka leftNode akan menjadi root baru
    } else if (node == node->parent->left) {
        node->parent->left = leftNode; //jika node berada di posisi anak kiri dari parent, maka leftNode yang akan menggantikannya
    } else {
        node->parent->right = leftNode; //jika node berada di posisi anak kanan parent, maka leftNode akan menjadi anak kanan baru dari parent
    }
    
    //sekarang leftNode manjadi parent dari node, sehingga diperlukan update sebagai berikut
    leftNode->right = node; //node menjadi anak kanan dari leftNode
    node->parent = leftNode; //parent dari node adalah leftNode
    
    //mengupdate root
    return root;
}

//fungsi untuk mengupdate tree, sehingga warna dan tree tidak memiliki violation sama sekali
struct RBT* update(struct RBT* root, struct RBT* node) {
	//akan mengulang proses jika node masih memiliki parent yang berwarna merah
    while (node->parent != NULL && node->parent->color == 1) {
    	//mengisi node uncle dengan mengecek apakah uncle tersebut merupakan anak kiri atau kanan dari grandparent (node->parent->parent)
    	struct RBT* uncle = (node->parent->parent->right == node->parent)? node->parent->parent->left : node->parent->parent->right;
    	//jika node saat ini memiliki uncle dan unclenya berwarna merah, maka hanya perlu mengupdate warna saja
    	if (uncle != NULL && uncle->color == 1) {
            node->parent->color = 0;
            uncle->color = 0;
            node->parent->parent->color = 1;
            //grandparent akan menjadi node terbaru untuk menghilangkan violation yang masih ada
            node = node->parent->parent;
        } 
        //jika parent node saat ini adalah anak kiri dari grandparent
		else if (node->parent == node->parent->parent->left) {
			//dan jika node berada di anak kanan parent, hal ini menjadi left righ case dan membutuhkan double rotation
            if (node == node->parent->right) {
                node = node->parent;
                root = leftRotation(root, node);
            }
            //selanjutnya adalah left left case dengan melakukan rotasi dan mewarnai node untuk menghilangkan violation
            node->parent->color = 0;
            node->parent->parent->color = 1;
            root = rightRotation(root, node->parent->parent);
        } 
		//jika parent node saat ini adalah anak kanan dari grand parent
		else {
        	//jika node saat ini adalah anak kiri dari parent, maka akan menjadi right left case, diperlukan double rotation
	        if (node == node->parent->left) {
	            node = node->parent;
	            root = rightRotation(root, node);
	        }
	        //selanjutnya adalah right right case, melakukan rotasi kiri dan mewarnai kembali node untuk menghilangkan violation
	        node->parent->color = 0;
	        node->parent->parent->color = 1;
	        root = leftRotation(root, node->parent->parent);
        }
	}
    root->color = 0; //untuk memastikan bahwa root selalu berwarna hitam
    //mengupdate root
    return root;
}

//fungsi insertion berikut masih mirip dengan cara insert pada BST, yang membedakan adalah tambahan untuk mengupdate pointer anak curr menjadikan curr sebagai parent
struct RBT* insertion(struct RBT* curr, struct RBT* newNode) {
	if (curr == NULL) {
		return newNode;
	} else if (curr->key > newNode->key) {
		curr->left = insertion(curr->left, newNode);
		curr->left->parent = curr;
	} else if (curr->key < newNode->key) {
		curr->right = insertion(curr->right, newNode);
		curr->right->parent = curr;
	}
	return curr;
}

//fungsi ini untuk membungkus langkah insert dan update menjadi satu fungsi
struct RBT* insertAndUpdate(struct RBT* root, int key) {
    struct RBT* newNode = createNode(key);
    root = insertion(root, newNode);
    root = update(root, newNode);
    return root;
}

//fungsi untuk print tree secara inorder
void printInorder(struct RBT* node) {
    if (node != NULL) {
	    printInorder(node->left);
	    printf("%d ", node->key);
	    printInorder(node->right);
    }
}

void testInorderColor(struct RBT* curr) {
	if (curr != NULL) {
		testInorderColor(curr->left);
		printf("%d %s\n", curr->key, (curr->color)?"red":"black");
		testInorderColor(curr->right);
	}
}

int main() {
    struct RBT* root = NULL;
    int keyList[] = {41, 22, 5, 51, 48, 29, 18, 21, 45, 3};
	int size = sizeof(keyList)/sizeof(int);
	for (int i = 0; i < size; i++) {
		root = insertAndUpdate(root, keyList[i]);
		root->color = 0; //memastikan kembali bahwa root berwarna hitam (0 -> hitam)
	}
//	printf("root %d\n", root->key);
	printf("Inorder Traversal of Created Tree\n");
//	testInorderColor(root);
	printInorder(root);
	return 0;
}
