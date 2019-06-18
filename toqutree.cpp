
/**
 *
 * toqutree (pa3)
 * significant modification of a quadtree .
 * toqutree.cpp
 * This file will be used for grading.
 *
 */

#include "toqutree.h"
#include <stack>
#include <queue>

toqutree::Node::Node(pair<int,int> ctr, int dim, HSLAPixel a)
	:center(ctr),dimension(dim),avg(a),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

toqutree::~toqutree(){
	clear(root);
}

toqutree::toqutree(const toqutree & other) {
	root = copy(other.root);
}


toqutree & toqutree::operator=(const toqutree & rhs){
	if (this != &rhs) {
		clear(root);
		root = copy(rhs.root);
	}
	return *this;
}

toqutree::toqutree(PNG & imIn, int k){

/* This constructor grabs the 2^k x 2^k sub-image centered */
/* in imIn and uses it to build a quadtree. It may assume  */
/* that imIn is large enough to contain an image of that size. */

/* your code here */
PNG* p = new PNG();
p->resize(pow(2,k),pow(2,k));
for (auto i = 0; i< pow(2,k); i++) {
	for (auto j = 0; j< pow(2,k); j++) {
		*(p ->getPixel(i,j)) = *(imIn.getPixel(imIn.width()/2 -((int)pow(2,k))/2 + i,imIn.height()/2 - ((int)pow(2,k))/2+j));
	}
}

root = buildTree(&imIn,k);
delete p;

}

int toqutree::size() {
	if (root == NULL) return 0;
	return sizehelper(root);
}

int toqutree:: sizehelper(Node* n) {
	int retVal = 1;
 if (n -> NW != NULL) {
	 retVal += sizehelper(n -> NW);
 }
 if (n -> NE != NULL) {
	 retVal += sizehelper(n->NE);
 }
 if (n -> SW != NULL) {
	 retVal += sizehelper(n->SW);
 }
 if (n -> SE != NULL) {
	 retVal += sizehelper(n->SE);
 }
return retVal;
//return (pow(4,root->dimension+1) - 1)/3;

}


toqutree::Node * toqutree::buildTree(PNG * im, int k) {

/* your code here */

// Note that you will want to practice careful memory use
// In this function. We pass the dynamically allocated image
// via pointer so that it may be released after it is used .
// similarly, at each level of the tree you will want to
// declare a dynamically allocated stats object, and free it
// once you've used it to choose a split point, and calculate
// an average.

if (k == 0)
	return new Node(pair<int,int>(0,0), k,*(im->getPixel(0,0)));
if (k < 0)
	return NULL;
stats* s = new stats(*im);
int size = pow(2, k - 1);
pair<int,int> correctloc = buildTree(im, pair<int, int>(0, 0), pair<int, int>(im->width()-1, im->height() - 1), s, k-1);
	Node* p = new Node(correctloc,k,s->getAvg(pair<int,int>(0,0), pair<int, int>(im->width()-1, im->height() - 1)));
	delete s;

	//Split up to 4 sub-images.
PNG img1(size,size);
for (int x = 0; x < size; x++) {
	for (int y = 0; y < size; y++) {
		*(img1.getPixel(x,y)) = *(im->getPixel((x+correctloc.first) % im->width(), (y+correctloc.second)%im->height()));
		//cout <<x<<"\t"<<y<<"\t"<< (x+correctloc.first) % im->width() <<"\t" <<(y+correctloc.second)%im->height() <<endl;
	}
}
p->SE = buildTree(&img1, k-1);

	for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
					*(img1.getPixel(x,y)) = *(im->getPixel((x+correctloc.first+size) % im->width(), (y + correctloc.second)%im->height()));
			}
	}
	p->SW = buildTree(&img1, k-1);

	for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
					*(img1.getPixel(x,y)) = *(im->getPixel(  (x+correctloc.first) % im->width(), (y+correctloc.second+size)%im->height()));
			}
	}
	p->NE = buildTree(&img1, k-1);

	for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
					*(img1.getPixel(x,y)) = *(im->getPixel(  (x+correctloc.first+size) % im->width(), (y+correctloc.second+size)%im->height()));
			}
	}
	p->NW = buildTree(&img1, k-1);

return p;

}


pair<int,int> toqutree::buildTree(PNG * im, pair<int, int> ul, pair<int, int> lr, stats* s ,int dim) {
//min entropy
double mine = std::numeric_limits<double>::max();
//Location of min entropy
pair<int, int> loc(0,0);
//return (0,0) for dimension 0
	if(dim==0)
		return loc;
	//side Length of the sub image;
int sidelentth = pow(2, dim);
//side length of the canvas.
int bdlength = lr.first - ul.first+1;
assert(bdlength = pow(2, dim+1));
assert(dim>=1);

//X and Y starts from 1/4 to 3/4
for (int x = im->width()/4; x < (int)im->width()*3/4; x++) {
	for (int y = im->height()/4; y < (int)im->height()*3/4; y++) {
		//Calculate entropy for 4 squares.
		double t0 = entP(im, ul, lr, pair<int, int>(x, y),
						pair<int, int>(x + sidelentth - 1, y+ sidelentth - 1), s);

		double t1 = entP(im, ul, lr, pair<int, int>((x+ sidelentth)% bdlength, y),
			pair<int, int>((x + sidelentth) % bdlength + sidelentth - 1, y + sidelentth - 1), s);

		double t2 = entP(im, ul, lr, pair<int, int>(x, (y+ sidelentth) % bdlength),
			pair<int, int>(x + sidelentth - 1, (y + sidelentth) % bdlength + sidelentth - 1), s);

					double t3 = entP(im, ul, lr, pair<int, int>((x+ sidelentth) % bdlength, (y + sidelentth) % bdlength),
			pair<int, int>((x + sidelentth) % bdlength + sidelentth - 1, (y + sidelentth) % bdlength + sidelentth - 1), s);
					assert(t0>=0);
					assert(t1>=0);
					assert(t2>=0);
					assert(t3>=0);

		double t = ((double)(t0 + t1 + t2 + t3));
		//compare
		if (t <= mine) {
			mine = t;
			loc = pair<int,int>(x, y);
		}
	}
}
return loc;
}


double toqutree::entP(PNG * im, pair<int, int> ul, pair<int, int> lr, pair<int, int> locul, pair<int,int> loclr, stats* s) {
vector<int> dist;
long area = im->width()*im->height() / 4;
vector<vector<int>> temp;
if (loclr.first > lr.first && loclr.second > lr.second) {
	//If there are 4 seperate regions
	loclr = pair<int, int>(loclr.first - (im->width()), loclr.second - (im->height()));
	temp.push_back(s->buildHist(locul,lr));
	temp.push_back(s->buildHist(ul, loclr));
	temp.push_back(s->buildHist(pair<int, int>(locul.first, 0), pair<int, int>(im->width()-1, loclr.second)));
	temp.push_back(s->buildHist(pair<int, int>(0, locul.second), pair<int, int>(loclr.first, im->height()-1)));
}
else if (loclr.first > lr.first) {
	//If x value is out of bound
	loclr = pair<int, int>(loclr.first - (im->width()), loclr.second);
	temp.push_back(s->buildHist(locul, pair<int, int>(im->width()-1, loclr.second)));
	temp.push_back(s->buildHist(pair<int, int>(0, locul.second), loclr));
}
else if (loclr.second > lr.second) {
	//Y is out of bound
	loclr = pair<int, int>(loclr.first, loclr.second - (im->height()));
	temp.push_back(s->buildHist(locul, pair<int, int>(loclr.first, lr.second)));
	temp.push_back(s->buildHist(pair<int, int>(locul.first, ul.second), loclr));
}
else {
	//One piece
	return s->entropy(locul, loclr);
}
assert(temp.size()<=4);
long a=0;
dist = addHist(temp);
//Calculate the sum in Hist.
	for(int i=0;i<(int)dist.size();i++)
			a+=dist[i];
	assert(a==area);
return s->entropy(dist, area);
}


vector<int> toqutree::addHist(vector<vector<int>> s) {
//append the Hist vectors
vector<int> e(36);
	for (int h = 0; h < 36; h++) {
			e[h] = 0;
		for (int i = 0; i <(int) s.size(); i++) {
		e[h] += s[i][h];
	}

}
return e;
}


PNG toqutree::render(){

// My algorithm for this problem included a helper function
// that was analogous to Find in a BST, but it navigated the
// quadtree, instead.

/* your code here */
    cout<<"Start Render" <<endl;
	PNG img;
	img.resize(pow(2,root->dimension),pow(2,root->dimension));
	r(img,root);
	return img;
}

void toqutree::r(PNG &im, toqutree::Node* node) {

	if(node->SE == NULL){
		//If the node is a leaf;
		int size = pow(2, node->dimension);
		for(int i=0; i<size;i++)
			for(int j=0; j<size;j++)
				*(im.getPixel(i,j)) = node->avg;
		return;
	}
	int size = pow(2, node->dimension);
	int sub_size = pow(2, node->dimension -1 );

	pair<int,int> SEul = node->center;
	//Initialize PNG
	PNG SEI;
	SEI.resize(sub_size,sub_size);

	r(SEI,node->SE);

	//Paint the pixels on to the image
	for(int x =0; x< sub_size;x++){
		for(int y =0 ; y< sub_size; y++){
			int coox = x+SEul.first;
			int cooy = y+SEul.second;
			*(im.getPixel(coox%size, cooy%size)) = *(SEI.getPixel(x,y));
		}
	}

    r(SEI,node->SW);

    for(int x =0; x< sub_size;x++){
        for(int y =0 ; y< sub_size; y++){
            int coox = x+SEul.first+sub_size;
            int cooy = y+SEul.second;
            *(im.getPixel(coox%size, cooy%size)) = *(SEI.getPixel(x,y));
        }
    }

    r(SEI,node->NE);


    for(int x =0; x< sub_size;x++){
        for(int y =0 ; y< sub_size; y++){
            int coox = x+SEul.first;
            int cooy = y+SEul.second+sub_size;
            *(im.getPixel(coox%size, cooy%size)) = *(SEI.getPixel(x,y));
        }
    }

     r(SEI,node->NW);

    for(int x =0; x< sub_size;x++){
        for(int y =0 ; y< sub_size; y++){
            int coox = x+SEul.first+sub_size;
            int cooy = y+SEul.second+sub_size;
            *(im.getPixel(coox%size, cooy%size)) = *(SEI.getPixel(x,y));
        }
    }
   // im.writeToFile("images/"+to_string(size)+"NW.png");

}

/* oops, i left the implementation of this one in the file! */
void toqutree::prune(double tol){

	prune(tol,root);

}


int toqutree::prune(double tol, toqutree::Node* node){

queue<toqutree::Node*> layer;
layer.push(node);
while (!layer.empty()){
		Node* nt = layer.front();
		layer.pop();

		if(nt == NULL){
					continue;
		}
			HSLAPixel t0 = nt->avg;
			stack<toqutree::Node*> nodes;
			nodes.push(nt);
			int p = 1;
			while(!nodes.empty()){
					Node* n = nodes.top();
					nodes.pop();
					if(n==NULL)
							continue;
					if(n->dimension == 0){
							if(n->avg.dist(t0) > tol){
									p=0;
									break;
							}
					}
					nodes.push(n->NW);
					nodes.push(n->NE);
					nodes.push(n->SW);
					nodes.push(n->SE);
			}
			if(p == 0){
					layer.push(nt->SE);
					layer.push(nt->SW);
					layer.push(nt->NE);
					layer.push(nt->NW);
					continue;
			}else{
					clear(nt->SE);
					clear(nt->SW);
					clear(nt->NE);
					clear(nt->NW);
			}

			continue;
}
return 0;

}


/* called by destructor and assignment operator*/
void toqutree::clear(Node * & curr){

	if(curr == NULL)
		return;
	else{
		clear(curr->SE);
		clear(curr->SW);
		clear(curr->NE);
		clear(curr->NW);
		delete curr;
		curr = NULL;
	}

}

/* done */
/* called by assignment operator and copy constructor */
toqutree::Node * toqutree::copy(const Node * other) {
 // root = copyTree(other);
 // return root;
 if(other == NULL)
	 return NULL;
 Node* n = new Node(other->center, other->dimension, other->avg);
 n->SE = copy(other->SE);
 n->SW = copy(other->SW);
 n->NE = copy(other->NE);
 n->NW = copy(other->NW);
 return n;
}
