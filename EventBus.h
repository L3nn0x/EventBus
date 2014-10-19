/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventBus.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: availlan <availlan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/19 12:59:04 by availlan          #+#    #+#             */
/*   Updated: 2014/10/19 22:27:23 by availlan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef EVENTBUS_H
# define EVENTBUS_H

#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>
#include <algorithm>

namespace EventBus
{
	class   BaseMessage
	{
		public:
		BaseMessage(std::string id);
		virtual ~BaseMessage();

		unsigned int    getId() const { return m_id; }

		protected:
		unsigned int    m_id;
	};

	template<typename T>
	class   Message : public BaseMessage
	{
		public:
		Message(std::string id, T* data) : BaseMessage(id), m_data(data)
		{}
		virtual ~Message()
		{}

		T   *getData() const { return m_data.get(); }

		protected:
		std::auto_ptr<T>    m_data;
	};

	class   Bus
	{
		public:
		Bus();
		~Bus();

		void    addListener(std::string id, std::function<void(BaseMessage*)> function);
		template<typename U>
		Message<U>    *sendMessage(Message<U> *message)
		{
			auto    range = listeners.equal_range(message->getId());
			for_each(range.first, range.second, [message] (std::unordered_multimap<unsigned int, std::function<void(BaseMessage*)>>::value_type& i)
			{
				i.second(message);
			});
			return message;
		}

		protected:
		std::unordered_multimap<unsigned int, std::function<void(BaseMessage*)>>  listeners;
	};
}

#endif /* !EVENTBUS_H */
