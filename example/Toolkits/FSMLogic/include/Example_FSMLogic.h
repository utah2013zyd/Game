#ifndef __Orz_Example_FSMLogic__
#define __Orz_Example_FSMLogic__
#include <orz/Toolkit_Base/FSMLogic.h>

namespace sc = boost::statechart;
namespace mpl = boost::mpl;
namespace Orz
{

	struct EvStartStop : sc::event< EvStartStop > {};
	struct EvReset : sc::event< EvReset > {};
	struct EvGetElapsedTime : sc::event< EvGetElapsedTime >
	{
	public:
		EvGetElapsedTime( double & time ) : time_( time ) {}
		void Assign( double time ) const{time_ = time;}
	private:
		double & time_;
	};

	class Active;
	class GameEntity//: private FSMLogic<GameEntity, Active>
	{
	public:
		GameEntity(void)
		{
			//_logic_initiate();
			_logic.init(this);
		}

		void update(TimeType i)
		{
			//_logic_update(i);
			_logic.update(i);
		}
		void startStop(void)
		{
			//_logic_process_event(EvStartStop());
			_logic.process_event(EvStartStop());
		}

		void reset(void)
		{
			//_logic_process_event(EvReset());
			_logic.process_event(EvReset());
		}
		double getElapsedTime(void)
		{
			double time = 0;
			//_logic_process_event(EvGetElapsedTime(time));
			
			_logic.process_event(EvGetElapsedTime(time));
			return time;
		}

		void print(const std::string & text)
		{
			std::cout<<text<<std::endl;
		}

		~GameEntity(void)
		{
			//_logic_shutdown();
			_logic.shutdown();
		};
	private:
		FSMLogicHandler<GameEntity, Active> _logic;
	};




	class Active;
	class Stopped;






	class Active :public  FSM::MainLogic< Active, GameEntity, Stopped >
	{
	public:
		typedef mpl::list<sc::custom_reaction< UpdateEvt >, sc::transition< EvReset, Active > >  reactions;

		Active() : elapsedTime_( 0.0 ) {}

		double & ElapsedTime(){return elapsedTime_;}

		double ElapsedTime() const
		{
			return elapsedTime_;
		}

		sc::result react( const UpdateEvt & evt )
		{
			GameEntity * entity = getOwner();
			entity->print("Update!");
			return discard_event();
		}
	private:
		double elapsedTime_;
	};

	class Running :public  FSM::Logic< Running, Active >
	{
	public:
		typedef mpl::list<
			sc::custom_reaction< EvGetElapsedTime >,
			sc::transition< EvStartStop, Stopped >
		> reactions;

		Running() : startTime_( std::time( 0 ) ) {}

		~Running()
		{

			context< Active >().ElapsedTime() = ElapsedTime();
		}

		sc::result react( const EvGetElapsedTime & evt )
		{
			GameEntity * entity = getOwner();
			entity->print("Get Elapsed Time!");


			evt.Assign( ElapsedTime() );
			return discard_event();
		}

	private:
		double ElapsedTime() const
		{
			return context< Active >().ElapsedTime() +
				std::difftime( std::time( 0 ), startTime_ );
		}

		std::time_t startTime_;
	};

	class Stopped :public FSM::Logic< Stopped, Active >
	{
	public:
		typedef mpl::list<
			sc::custom_reaction< EvGetElapsedTime >,
			sc::transition< EvStartStop, Running >
		> reactions;

		sc::result react( const EvGetElapsedTime & evt )
		{	

			evt.Assign( context< Active >().ElapsedTime() );
			return discard_event();
		}
	};




}
#endif