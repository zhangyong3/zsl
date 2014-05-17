#ifndef _ZSL_MAP_H
#define _ZSL_MAP_H

#include "set.h"
#include <utility>

namespace zsl {


template<class K, class V, class C=std::less<std::pair<K,V> > >
class Map : public Set<std::pair<K,V>, C>
{
protected:
	typedef typename Set<std::pair<K,V>, C>::_SetNode SetNodeType;
	typedef Set<std::pair<K,V>,C> SetType;
public:
	typedef typename Set<std::pair<K,V>,C>::Iterator Iterator;
	Iterator find(const K &key)
	{
		return SetType::find(std::make_pair(key, V()));
	}

	V &operator [](const K &key)
	{
		TreeNode *p;
		Iterator it = find(key);
		if (it == SetType::end()) {
			p = new SetNodeType(std::make_pair(key, V()));
			RBTree::insert(p);
		} else {
			p = it.cur;
		}
		return ((SetNodeType*)p)->data.second;
	}
};

}


#endif
