#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>
#include <algorithm>

namespace Event_bus
{
	template<typename T>
	class Message
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
		Bus();
		~Bus();

		void    Add_listener(std::string id, std::function<void(void*)> function)
		{
			std::hash<std::string>  hash;
			listeners.insert(std::pair<unsigned int, std::function<void(void*)>>(hash(id), function));
		}

		template<typename T>
		void    Send_message(Message<T> message)
		{
			for (auto i : listeners.equal_range(message.Get_id()))
			{
				i->second(message.Get_data());
			}
		}

		protected:
		std::unordered_multimap<unsigned int, std::function<void(void*)>>  listeners;
	};
}

int main()
{
	std::cout<<"Creating message"<<std::endl;
	Event_bus::Message<std::string> m("record", std::string("hello"));
	Event_bus::Bus  b();
	b.Add_listener("record", [] (void *i) { std::string *tmp = static_cast<std::string*>(i); std::cout<<*tmp<<std::endl;});
	b.Send_message(m);
}
