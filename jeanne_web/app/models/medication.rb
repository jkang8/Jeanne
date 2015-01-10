def decode(string)
	return ActiveSupport::JSON.decode string
end


class Medication < ActiveRecord::Base
	belongs_to :patient


	def time_array
		if times == ''
			return []
		end
		return decode times
	end

	def days_array
		if not days 
			return []
		end
		return decode days
	end

	def week_days 
		map = ['M', 'T', 'W', 'Th', 'F', 'S', 'Su']

		days = []
		days_array.each do | day |
			days << map[day]
		end

		return days
	end

	def everyday?
		week_days.count > 6
	end
	

end
