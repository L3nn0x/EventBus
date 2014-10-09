#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>
#include <algorithm>

namespace Event_bus
{
	class   Base_message
	{
		public:
		virtual ~Base_message() {}
	};
	
	template<typename T>
	class Message : public Base_message
	{
		public:
		Message(std::string id, T data) : m_data(new T(data))
		{
			std::hash<std::string> hash;
        
			m_id = hash(id);
		}

		T*  Get_data(void) const { return m_data.get(); }

		unsigned int    Get_id(void) const { return m_id; }

		protected:
			unsigned int  m_id;
			std::auto_ptr<T> m_data;
	};
  
	class Bus
	{
		public:
		Bus() {}
		~Bus() {}

		void    Add_listener(std::string id, std::function<void(Base_message*)> function)
		{
			std::hash<std::string>  hash;
			listeners.insert(std::pair<unsigned int, std::function<void(Base_message*)>>(hash(id), function));
		}

		template<typename U>
		Message<U>    *Send_message(Message<U> *message)
		{
			auto    range = listeners.equal_range(message->Get_id());
			for_each(range.first, range.second, [message] (std::unordered_multimap<unsigned int, std::function<void(Base_message*)>>::value_type& i)
			{
				i.second(message);
			});
			return message;
		}

		protected:
		std::unordered_multimap<unsigned int, std::function<void(Base_message*)>>  listeners;
	};
}

int main()
{
	std::cout<<"Creating message"<<std::endl;
	Event_bus::Message<std::string> *m = new Event_bus::Message<std::string>("record", std::string("hello"));
	Event_bus::Bus  *b = new Event_bus::Bus();
	b->Add_listener("record", [] (Event_bus::Base_message *i) { Event_bus::Message<std::string> *tmp = static_cast<Event_bus::Message<std::string>*>(i); std::cout<<"Premiere foo :\n"<<*tmp->Get_data()<<std::endl; });
	b->Add_listener("record", [] (Event_bus::Base_message *i) { Event_bus::Message<std::string> *tmp = static_cast<Event_bus::Message<std::string>*>(i); std::cout<<"deuxieme foo :\n"<<*tmp->Get_data()<<std::endl; });
	b->Add_listener("plop", [] (Event_bus::Base_message *i) { Event_bus::Message<std::string> *tmp = static_cast<Event_bus::Message<std::string>*>(i); std::cout<<"troisieme foo :\n"<<*tmp->Get_data()<<std::endl; });
	b->Send_message(m);
	delete b->Send_message(new Event_bus::Message<std::string>("plop", std::string("plop")));
	delete b;
}
