class SuffixTree26 {
private:
	int n;
	string s;
	// s[l, r) := the substring indicated by the edge toward this node from its parent.
	// dep := the length of the string from root to this node
	// cnt := #{the strings at this node}
	// suf := the deepest node which is a suffix of this node
	struct node_t {
		int l; int r; array<node_t*, 26> childs; int dep; node_t *parent; int cnt; node_t *suf;
	};
	const char base = 'a';
	node_t *root;
	vector<node_t*> sufnodes;
	inline node_t* proceed(node_t *cur, int &k, int &i) {
		if (i == n) return cur;
		if (k == cur->r - cur->l) {
			if (cur->childs[s[i] - base] == nullptr) return cur;
			else {
				cur = cur->childs[s[i] - base];
				k = 1;
				i++;
				return proceed(cur, k, i);
			}
		}
		else {
			if (s[cur->l + k] != s[i]) return cur;
			else {
				k++;
				i++;
				return proceed(cur, k, i);
			}
		}
	}
	inline node_t* insert(node_t *cur, const int k, const int i) {
		if (k < cur->r - cur->l) {
			node_t *branch = new node_t{ cur->l, cur->l + k, {}, cur->parent->dep + k, cur->parent, 0, nullptr };
			branch->childs[s[cur->l + k] - base] = cur;
			branch->parent->childs[s[cur->l] - base] = branch;
			cur->l = cur->l + k;
			cur->parent = branch;
			cur = branch;
		}
		if (i < n) {
			node_t *leaf = new node_t{ i, n, {}, cur->dep + n - i, cur, 1, nullptr };
			cur->childs[s[i] - base] = leaf;
			sufnodes[leaf->dep] = leaf;
		}
		else {
			cur->cnt++;
			sufnodes[cur->dep] = cur;
		}
		return cur;
	}
	inline node_t* proceed_suf(node_t *pre, int &k) {
		if (pre == root) return root;
		node_t *cur;
		int j;
		if (pre->parent == root) {
			cur = root;
			j = 1;
		}
		else {
			cur = pre->parent->suf;
			j = 0;
		}
		k = cur->r - cur->l;
		while (j < pre->r - pre->l) {
			cur = cur->childs[s[pre->l + j] - base];
			k = pre->r - pre->l - j;
			if (k > cur->r - cur->l) {
				j += cur->r - cur->l;
			}
			else {
				break;
			}
		}
		return cur;
	}
	void construct() {
		node_t *cur = root;
		node_t *pre = nullptr;
		int k = 0;
		int i = 0;
		root->suf = root;
		sufnodes[0] = root;
		while (i != n || cur != root) {
			cur = proceed(cur, k, i);
			cur = insert(cur, k, i);
			if (pre != nullptr) {
				pre->suf = cur;
			}
			pre = cur;
			if (pre == root) i++;
			cur = proceed_suf(pre, k);
			if (k == cur->r - cur->l) {
				pre->suf = cur;
				pre = nullptr;
			}
		}
	}
public:
	SuffixTree26(const string &s) {
		this->root = new node_t{ 0, 0, {}, 0, nullptr, 1, nullptr };
		this->n = s.length();
		this->s = s;
		this->sufnodes = vector<node_t*>(n + 1, nullptr);
		this->construct();
		return;
	}
	// decide if t is a suffix of s, including ""
	bool find_suffix(const string &t) {
		node_t *a = root;
		int k = 0;
		Loop(i, t.length()) {
			if (k == a->r - a->l) {
				a = a->childs[t[i] - base];
				k = 0;
				if (a == nullptr) return false;
			}
			if (t[i] == s[a->l + k]) {
				k++;
			}
			else {
				return false;
			}
		}
		if (k == a->r - a->l) {
			return a->cnt;
		}
		else {
			return false;
		}
	}

};