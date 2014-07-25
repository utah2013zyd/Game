
#ifndef __Orz_ToolkitBase_FSMLogic_h__
#define __Orz_ToolkitBase_FSMLogic_h__
#pragma warning(push)
#pragma warning(disable:4355)
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
namespace sc = boost::statechart; 
namespace Orz
{


	template<class Owner, class GameLogic>
	class __AttachingLogic: public boost::statechart::state_machine<__AttachingLogic<Owner, GameLogic>, GameLogic>
	{
	public:
		typedef Owner owner_type;
	public:
		inline __AttachingLogic(Owner * owner):_owner(owner){}
		inline  ~__AttachingLogic(void){}
		/// 得到其所有者
		inline Owner * getOwner(void){return _owner;}

	private:
		Owner * _owner;

	};


	class UpdateEvt: public boost::statechart::event<UpdateEvt>
	{
	public:
		UpdateEvt(TimeType interval):_interval(interval){}
		///得到两次调用的时间间隔
		TimeType getInterval(void) const {return _interval;}
	private:
		TimeType _interval;
	};

	template<class Owner, class GameLogic, class StateMachine = __AttachingLogic<Owner, GameLogic> >
	class FSMLogicHandler
	{
	public:
		FSMLogicHandler(void)
		{
		}
		///初始化有限状态机
		void init(Owner * owner)
		{
			_stateMachine.reset(new StateMachine(owner));
			_stateMachine->initiate();
		}

		///关闭有限状态机
		void shutdown(void)
		{
			_stateMachine->terminate();
			_stateMachine.reset();
		}

		///传递消息给有限状态机
		template <class T>
		void process_event(const T & evt)
		{
			_stateMachine->process_event(evt);
		}

		///更新有限状态机逻辑，参数是两次调用的时间间隔
		void update(TimeType interval = 0.0f)
		{
			process_event(UpdateEvt(interval));
		}

		///的到有限状态机的引用
		StateMachine & getStateMachine(void)
		{
			return (*_stateMachine.get());
		}
	private:
		boost::scoped_ptr<StateMachine> _stateMachine;
	};


	template<class Self, class GameLogic, class StateMachine = __AttachingLogic<Self, GameLogic> >
	class  FSMLogic
	{
	public:
		FSMLogic(void)
		{
		}
		///初始化有限状态机
		void _logic_initiate(void)
		{
			_stateMachine.reset(new StateMachine(static_cast<Self*>(this)));
			_stateMachine->initiate();
		}

		///关闭有限状态机
		void _logic_shutdown(void)
		{
			_stateMachine->terminate();
			_stateMachine.reset();
		}

		///传递消息给有限状态机
		template <class T>
		void _logic_process_event(const T & evt)
		{
			_stateMachine->process_event(evt);
		}

		///更新有限状态机逻辑，参数是两次调用的时间间隔
		void _logic_update(TimeType interval = 0.0f)
		{
			_logic_process_event(UpdateEvt(interval));
		}

		///的到有限状态机的引用
		StateMachine & getStateMachine(void)
		{
			return (*_stateMachine.get());
		}
	private:
		boost::scoped_ptr<StateMachine> _stateMachine;

	};



	namespace FSM
	{
		template<
			class GameLogic, 
			class Owner, 
			class InnerInitial = boost::mpl::list<> ,//进入第一个子状态
				boost::statechart::history_mode historyMode = boost::statechart::has_no_history
		>	
		class MainLogic: public boost::statechart::simple_state<GameLogic,  __AttachingLogic<Owner, GameLogic>, InnerInitial, historyMode> //总的逻辑
		{
		public:
			//typedef typename OwnerOrStateMachine::owner_type owner_type;
			typedef __AttachingLogic<Owner, GameLogic>  state_machine_type;
			//typedef OwnerOrStateMachine  state_machine_type;
		protected:

			///得到其所有者的指针
			inline typename state_machine_type::owner_type * getOwner(void)
			{
				return getStateMachine().getOwner();
			}

			///的到有限状态机的引用
			inline state_machine_type & getStateMachine(void)
			{
				return context< state_machine_type >();
			}
		};

		template<
			class GameLogic, 
			class CustomStateMachine, 
			class InnerInitial,//进入第一个子状态
			boost::statechart::history_mode historyMode
		>
		class MainLogicWithCustomStateMachine:
			public boost::statechart::simple_state<GameLogic, CustomStateMachine, InnerInitial, historyMode> //总的逻辑
		{
		public:
			//typedef OwnerOrStateMachine owner_type;
			typedef CustomStateMachine  state_machine_type;
		protected:
			
			///得到其所有者的指针
			inline typename state_machine_type::owner_type * getOwner(void)
			{
				return getStateMachine().getOwner();
			}

			///的到有限状态机的引用
			inline state_machine_type & getStateMachine(void)
			{
				return context< state_machine_type >();
			}
		};



		template<
			class GameLogic, 
			class Father, 
			class InnerInitial = boost::mpl::list<> ,//进入第一个子状态
			boost::statechart::history_mode historyMode = boost::statechart::has_no_history
		>	
		class Logic: public boost::statechart::simple_state<GameLogic, Father, InnerInitial, historyMode> //普通逻辑
		{
		public:
			//typedef typename Father::owner_type owner_type;
			typedef typename Father::state_machine_type  state_machine_type;
		protected:
			
			///得到其所有者的指针
			inline typename state_machine_type::owner_type * getOwner(void)
			{
				return getStateMachine().getOwner();
			}

			///的到有限状态机的引用
			inline state_machine_type & getStateMachine(void)
			{
				return context< state_machine_type >();
			}
		};



	}

}

#pragma warning(pop)
#endif
