#include<iostream>
#include<bits/stdc++.h>
using namespace std;
string s[100];//读文件存节点
int i = 0;//读文件时记录节点个数
bool flag = false;//删除清空后用来确定 创建ancestor   
struct TreeNode{
    string name;
    struct TreeNode *lChild;
    struct TreeNode *rChild;
};
   
//文件操作：
//家谱记录的输入(要求在输入家谱记录时按从祖先到子孙的顺序输入，
//第一个家谱记录的父亲域为所有人的祖先。)   
//输入存父母孩子
TreeNode *savePerson(){
    cout<<"输入父母孩子名字，以空格分开:";
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
}//女只用右孩子，男左边妻子，右边儿子

//遍历查找人名 
TreeNode* Find(TreeNode *node,string name){
    if(node==NULL)return NULL;
    if(node->name==name)return node;
    TreeNode *temp = Find(node->lChild,name);
    if(temp!=NULL)return temp;
    temp = Find(node->rChild,name);
    if(temp!=NULL)return temp;
    return NULL;
}

//存小孩父母 如果同一父母
void save(TreeNode *root){
    //这是家谱中已有祖先的情况
    TreeNode *fam = savePerson();//父是根节点
    TreeNode *result = Find(root,fam->name);
    if(result!=NULL){
        if(result->lChild==NULL){
            //没老婆儿子 result是在家谱找到的父节点
            result->lChild = fam->lChild;
        }else{
            //有妻子儿子
            TreeNode *temp = result->lChild;//指向父老婆（母）
            while(temp->rChild!=NULL){
            temp = temp->rChild;
            }
            temp->rChild = fam->lChild->rChild; //fam中的孩子
        }
    }else if(result==NULL){
        cout<<"没有找到你的祖先,不存入"<<endl;
    }
}
//家谱记录的输出
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

//清除全部文件记录   释放树
void freeTree(TreeNode *node){
    if(node==NULL)return;
    freeTree(node->lChild);
    freeTree(node->rChild);
    delete(node);
    node=NULL;
}
void destroyFile(){
    fstream file("family.txt",ios::out);
    cout<<"清除文件完成"<<endl;
}

//将家谱记录存盘
// 树保存到文件夹
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


//从文件读树
void readFile(){//把文件中的树节点读进string数组
    ifstream f("family.txt",ios::in);
    int i = 0;
    while(!f.eof()){
        f>>s[i];
        i++;
    }
    f.close();
}
//建树
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
//用括号表示法输出家谱二叉树
void printBinaryTree(TreeNode *node){
    if(node){
        cout<<node->name; //根节点存在就输出
    }else return; //不存在，则返回
    if(node->lChild!=NULL||node->rChild!=NULL)cout<<'('; //左右子树存在任一就输出
    else return; //左右子树都不存在返回
    if(node->lChild){
        printBinaryTree(node->lChild); //左子树存在
    }
    if(node->rChild){
        cout<<','; 
        printBinaryTree(node->rChild);
    }
    cout<<')';
}

void ShowSon(TreeNode *node,string name){
    //这里要判断父亲还是母亲  父亲肯定有左孩子，母亲肯定没左孩子
    TreeNode *parent = Find(node,name);
    if(parent==NULL){cout<<"查无此人"<<endl;return;}
    TreeNode *temp = NULL;
    if(parent->lChild==NULL){ 
        temp = parent->rChild;//指向第一个孩子
        cout<<name<<"的孩子是：";
        while(temp!=NULL){
            cout<<temp->name<<" ";
            temp = temp->rChild;
        }
        cout<<endl;
    }else if(parent->lChild!=NULL){
        temp = parent->lChild->rChild;
        cout<<name<<"的孩子是：";
        while(temp!=NULL){
            cout<<temp->name<<" ";
            temp = temp->rChild;
        }
        cout<<endl;
    }
}
//查找某人的所有祖先（这里的祖先是指所设计的二叉树结构中某结点的所有祖先结点）
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
        cout<<"选择1创建新家谱，选择2用文件中存储的家谱"<<endl; 
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
                cout<<"文件为空"<<endl;
            }else{
                readFile();
                ancestor = BuildTree();
                break;
            }
            f.close();
        }
        else cout<<"输入错误"<<endl;
    }

    cout<<"-----------------------------家谱------------------------------"<<endl;
    while(true){
        char choose1 = -1;
        cout<<"按1选择文件操作，按2选择家谱操作，按3退出。你的选择是：";
        cin>>choose1;
        if(choose1=='1'){
            while(true){
                char choose2 = '-1';
                cout<<"按1选择输入家谱，按2选择输出家谱，按3选择清除记录返回，按4选择保存记录后返回。你的选择是：";
                cin>>choose2;
                if(choose2=='1'){
                    if(flag==false){
                        save(ancestor);
                    }else if(flag==true){
                        ancestor = savePerson();
                    }
                }else if(choose2=='2'){
                    if(ancestor==NULL){
                        cout<<"家谱为空"<<endl;
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
                            cout<<"家谱为空"<<endl;
                        }else{
                            //如果不为空，先清空文件夹
                            fstream f("family.txt",ios::out);
                            f.close();
                            saveInFile(ancestor);
                            cout<<"保存成功"<<endl;
                            break;  
                        }  
                }else {
                    cout<<"输入错误"<<endl;
                }
            }
        }else if(choose1=='2'){
            while(true){
                char choose3 = '-1';
                cout<<"按1选择括号表示法输出，按2选择查找某人的所有儿子，按3选择查找某人的所有祖先，按4选择返回。你的选择是：";
                cin>>choose3;
                if(choose3=='1'){
                    if(ancestor==NULL){
                        cout<<"家谱为空"<<endl;
                    }else{
                    printBinaryTree(ancestor);
                    cout<<endl;
                    }
                }else if(choose3=='2'){
                    if(ancestor==NULL){
                        cout<<"家谱为空"<<endl;
                    }else{
                    cout<<"请输入你要查找谁的儿子的名字"<<endl;
                    string s ;
                    cin>>s;
                    ShowSon(ancestor,s);
                    cout<<endl;
                    }
                }else if(choose3=='3'){
                    if(ancestor==NULL){
                        cout<<"家谱为空"<<endl;
                    }else{
                    cout<<"请输入你要查找谁的全部祖先"<<endl;
                    string s;
                    cin>>s;
                    cout<<s<<"的全部祖先是：";
                    ShowAllFather(ancestor,s);
                    cout<<endl;
                    }
                }else if(choose3=='4'){
                    break;
                }else {
                    cout<<"输出错误"<<endl;
                }
            }
        }else if(choose1=='3'){
            break;
        }else cout<<"输出错误"<<endl;
        cout<<"----------------------------------------------------------"<<endl;
    }
    system("pause");
    return 0;
}
