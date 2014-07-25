#ifndef __Orz_ToolkitPlus_Queue_hpp_
#define __Orz_ToolkitPlus_Queue_hpp_
#include <orz/Toolkit_Plus/Toolkit/ObjectList.hpp>
namespace Orz
{
/** linked-list style queue
 */
template<typename T>
class Queue
{
	struct Node
	{
		T element;
		Node* next;
	};

	ObjectList<Node>		m_nodes;

	Queue();

public:
	ORZ_PLUS_INLINE
	Queue(PtrIntType _maxNodeNum)
		: last(0)
		, first(0)
		, m_maxNodeNum(_maxNodeNum == 0 ? 1 : _maxNodeNum)
		, m_nodes(_maxNodeNum == 0 ? 1 : _maxNodeNum)
	{
	}

	ORZ_PLUS_INLINE
	~Queue()
	{
	}

public:
	ORZ_PLUS_INLINE
	void clear()
	{
		// clear any elements
		while(last != 0)
			pop();
	}

	ORZ_PLUS_INLINE
	void push(const T& _elem)
	{
		Node* n = 0;
		try
		{
			n = m_nodes.select();
		}
		catch(OrzAsioException& _e)
		{
			throw OrzAsioException(OrzAsioException::EXP_INVALID_STATE, ThrowAddress(__FILE__, __LINE__), _e);
		}
		catch(...)
		{
			throw OrzAsioException(OrzAsioException::EXP_INTERNAL_ERROR, ThrowAddress(__FILE__, __LINE__));
		}
		if (!n)
			return;
		
		if(last)
			last->next = n;
		else
			first = n;

		last = n;
		n->next = 0;
		n->element = elem;
	}

	ORZ_PLUS_INLINE
	void pop()
	{
		if(first == 0)
			return;
        
		Node* td = first;
		first = td->next;
		if(!first)
			last = 0;

		if (m_nodes.release(td) != 0)
			return;

		return;
	}

	ORZ_PLUS_INLINE
	bool pop(T& _t)
	{
		if(first == 0)
			return false;
        
		_t = first->element;
		Node* td = first;
		first = td->next;
		if(!first)
			last = 0;

		if (m_nodes.release(td) != 0)
			return false;

		return true;
	}

	ORZ_PLUS_INLINE
	T& front()
	{
		if (!first)
			throw InvalidParamsExp(InvalidParamsExp::IPEXP_NULL_PTR, ThrowAddress(__FILE__, __LINE__));
		return first->element;
	}

	ORZ_PLUS_INLINE
	T& back()
	{
		if (!last)
			throw InvalidParamsExp(InvalidParamsExp::IPEXP_NULL_PTR, ThrowAddress(__FILE__, __LINE__));
		return last->element;
	}

	ORZ_PLUS_INLINE
	bool empty()
	{
		return first == 0;
	}

private:
	Node*					last;
	Node*					first;
	PtrIntType			m_maxNodeNum;
};

}

#endif
