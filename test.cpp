#include <vector>
#include <iostream>
#include <string>
#include <functional>

namespace Event_bus
{
  class Message
  {
    public:
      Message(std::string id)
      {
        std::hash<std::string> hash;
        
        m_id = hash(id);
      }
      ~Message()
      {
          for (auto &i : m_data)
            delete i;
      }
    
      void  Add_data(any* data)
      {
        m_data.push_back(data);
      }
      
      void  print()
      {
          std::cout<<m_data[0]->Get_data();
      }
      
    protected:
      unsigned int  m_id;
      std::vector<void*> m_data;
  };
  
  class Bus
  {
    public:
      Bus();
      ~Bus();
  };
}

int main()
{
    std::cout<<"Creating message"<<std::endl;
    Event_bus::Message m("tada");
    m.Add_data(new Event_bus::Data<std::string>("salut"));
    m.print();
    std::cout<<"Done"<<std::endl;
}
