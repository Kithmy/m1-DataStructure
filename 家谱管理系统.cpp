#include<iostream>
#include<bits/stdc++.h>
using namespace std;
string s[100];//���ļ���ڵ�
int i = 0;//���ļ�ʱ��¼�ڵ����
bool flag = false;//ɾ����պ�����ȷ�� ����ancestor   
struct TreeNode{
    string name;
    struct TreeNode *lChild;
    struct TreeNode *rChild;
};
   
//�ļ�������
//���׼�¼������(Ҫ����������׼�¼ʱ�������ȵ������˳�����룬
//��һ�����׼�¼�ĸ�����Ϊ�����˵����ȡ�)   
//����游ĸ����
TreeNode *savePerson(){
    cout<<"���븸ĸ�������֣��Կո�ֿ�:";
    string s1,s2,s3;
    cin>>s1>>s2>>s3;
    TreeNode *father = new TreeNode;
    father->lChild = father->rChild = NULL;
    father->name = s1;
    TreeNode *mother = new TreeNode;
    mother->lChild = mother->rChild = NULL;
    mother->name = s2;
    TreeNode *son = new TreeNode;
    son->lChild = son->rChild = NULL;
    son->name = s3;
    father->lChild = mother;
    mother->rChild = son;
    return father;
}//Ůֻ���Һ��ӣ���������ӣ��ұ߶���

//������������ 
TreeNode* Find(TreeNode *node,string name){
    if(node==NULL)return NULL;
    if(node->name==name)return node;
    TreeNode *temp = Find(node->lChild,name);
    if(temp!=NULL)return temp;
    temp = Find(node->rChild,name);
    if(temp!=NULL)return temp;
    return NULL;
}

//��С����ĸ ���ͬһ��ĸ
void save(TreeNode *root){
    //���Ǽ������������ȵ����
    TreeNode *fam = savePerson();//���Ǹ��ڵ�
    TreeNode *result = Find(root,fam->name);
    if(result!=NULL){
        if(result->lChild==NULL){
            //û���Ŷ��� result���ڼ����ҵ��ĸ��ڵ�
            result->lChild = fam->lChild;
        }else{
            //�����Ӷ���
            TreeNode *temp = result->lChild;//ָ�����ţ�ĸ��
            while(temp->rChild!=NULL){
            temp = temp->rChild;
            }
            temp->rChild = fam->lChild->rChild; //fam�еĺ���
        }
    }else if(result==NULL){
        cout<<"û���ҵ��������,������"<<endl;
    }
}
//���׼�¼�����
void showFamily(TreeNode *node){

    if(node==NULL)return;
    if(node->lChild!=NULL){
        cout<<node->name<<" ";
        TreeNode *temp = node->lChild;
        while(temp!=NULL){
            cout<<temp->name<<" ";
            temp = temp->rChild;
        }
        cout<<endl;
        TreeNode *son = node->lChild->rChild;
        while(son!=NULL){
            showFamily(son);
            son = son->rChild;
        }
    }
}

//���ȫ���ļ���¼   �ͷ���
void freeTree(TreeNode *node){
    if(node==NULL)return;
    freeTree(node->lChild);
    freeTree(node->rChild);
    delete(node);
    node=NULL;
}
void destroyFile(){
    fstream file("family.txt",ios::out);
    cout<<"����ļ����"<<endl;
}

//�����׼�¼����
// �����浽�ļ���
void saveInFile(TreeNode *node){
    ofstream of("family.txt",ios::app);
    if(node==NULL){ 
        of<<"NULL"<<" ";
        return;
    }
    of<<node->name<<" ";
    of.close();
    saveInFile(node->lChild);
    saveInFile(node->rChild);
}


//���ļ�����
void readFile(){//���ļ��е����ڵ����string����
    ifstream f("family.txt",ios::in);
    int i = 0;
    while(!f.eof()){
        f>>s[i];
        i++;
    }
    f.close();
}
//����
TreeNode* BuildTree(){
    if(s[i]=="NULL"){
        i++;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        return NULL;
    }
    TreeNode *node = new TreeNode;
    node->lChild = node->rChild = NULL;
    node->name = s[i];
    i++;
    node->lChild = BuildTree();
    node->rChild = BuildTree();
    return node;
}
//�����ű�ʾ��������׶�����
void printBinaryTree(TreeNode *node){
    if(node){
        cout<<node->name; //���ڵ���ھ����
    }else return; //�����ڣ��򷵻�
    if(node->lChild!=NULL||node->rChild!=NULL)cout<<'('; //��������������һ�����
    else return; //���������������ڷ���
    if(node->lChild){
        printBinaryTree(node->lChild); //����������
    }
    if(node->rChild){
        cout<<','; 
        printBinaryTree(node->rChild);
    }
    cout<<')';
}

void ShowSon(TreeNode *node,string name){
    //����Ҫ�жϸ��׻���ĸ��  ���׿϶������ӣ�ĸ�׿϶�û����
    TreeNode *parent = Find(node,name);
    if(parent==NULL){cout<<"���޴���"<<endl;return;}
    TreeNode *temp = NULL;
    if(parent->lChild==NULL){ 
        temp = parent->rChild;//ָ���һ������
        cout<<name<<"�ĺ����ǣ�";
        while(temp!=NULL){
            cout<<temp->name<<" ";
            temp = temp->rChild;
        }
        cout<<endl;
    }else if(parent->lChild!=NULL){
        temp = parent->lChild->rChild;
        cout<<name<<"�ĺ����ǣ�";
        while(temp!=NULL){
            cout<<temp->name<<" ";
            temp = temp->rChild;
        }
        cout<<endl;
    }
}
//����ĳ�˵��������ȣ������������ָ����ƵĶ������ṹ��ĳ�����������Ƚ�㣩
bool ShowAllFather(TreeNode *node,string name){
    if(node==NULL)return false;
    if(node->name==name)return true;
    if(ShowAllFather(node->lChild,name)||ShowAllFather(node->rChild,name)){
        cout<<node->name<<" ";
        return true;
    }
    return false;
}

int main(){
    TreeNode *ancestor = NULL;
    char a;
    while(true){
        cout<<"ѡ��1�����¼��ף�ѡ��2���ļ��д洢�ļ���"<<endl; 
        cin>>a;
        if(a=='1'){
            ancestor = savePerson(); ;
            break;
        }
        else if(a=='2'){
            fstream f("family.txt");
            f.seekg(0,ios::end);
            streampos fp = f.tellg();
            if(int(fp)==0){
                cout<<"�ļ�Ϊ��"<<endl;
            }else{
                readFile();
                ancestor = BuildTree();
                break;
            }
            f.close();
        }
        else cout<<"�������"<<endl;
    }

    cout<<"-----------------------------����------------------------------"<<endl;
    while(true){
        char choose1 = -1;
        cout<<"��1ѡ���ļ���������2ѡ����ײ�������3�˳������ѡ���ǣ�";
        cin>>choose1;
        if(choose1=='1'){
            while(true){
                char choose2 = '-1';
                cout<<"��1ѡ��������ף���2ѡ��������ף���3ѡ�������¼���أ���4ѡ�񱣴��¼�󷵻ء����ѡ���ǣ�";
                cin>>choose2;
                if(choose2=='1'){
                    if(flag==false){
                        save(ancestor);
                    }else if(flag==true){
                        ancestor = savePerson();
                    }
                }else if(choose2=='2'){
                    if(ancestor==NULL){
                        cout<<"����Ϊ��"<<endl;
                    }else{
                    showFamily(ancestor);
                    cout<<endl;
                    }
                }else if(choose2=='3'){
                    freeTree(ancestor);
                    destroyFile();
                    ancestor = NULL;
                    flag = true;
                    break;
                }else if(choose2=='4'){
                        if(ancestor==NULL){
                            cout<<"����Ϊ��"<<endl;
                        }else{
                            //�����Ϊ�գ�������ļ���
                            fstream f("family.txt",ios::out);
                            f.close();
                            saveInFile(ancestor);
                            cout<<"����ɹ�"<<endl;
                            break;  
                        }  
                }else {
                    cout<<"�������"<<endl;
                }
            }
        }else if(choose1=='2'){
            while(true){
                char choose3 = '-1';
                cout<<"��1ѡ�����ű�ʾ���������2ѡ�����ĳ�˵����ж��ӣ���3ѡ�����ĳ�˵��������ȣ���4ѡ�񷵻ء����ѡ���ǣ�";
                cin>>choose3;
                if(choose3=='1'){
                    if(ancestor==NULL){
                        cout<<"����Ϊ��"<<endl;
                    }else{
                    printBinaryTree(ancestor);
                    cout<<endl;
                    }
                }else if(choose3=='2'){
                    if(ancestor==NULL){
                        cout<<"����Ϊ��"<<endl;
                    }else{
                    cout<<"��������Ҫ����˭�Ķ��ӵ�����"<<endl;
                    string s ;
                    cin>>s;
                    ShowSon(ancestor,s);
                    cout<<endl;
                    }
                }else if(choose3=='3'){
                    if(ancestor==NULL){
                        cout<<"����Ϊ��"<<endl;
                    }else{
                    cout<<"��������Ҫ����˭��ȫ������"<<endl;
                    string s;
                    cin>>s;
                    cout<<s<<"��ȫ�������ǣ�";
                    ShowAllFather(ancestor,s);
                    cout<<endl;
                    }
                }else if(choose3=='4'){
                    break;
                }else {
                    cout<<"�������"<<endl;
                }
            }
        }else if(choose1=='3'){
            break;
        }else cout<<"�������"<<endl;
        cout<<"----------------------------------------------------------"<<endl;
    }
    system("pause");
    return 0;
}
