class Patient < ActiveRecord::Base
	has_many :caregivers
	has_many :medications

	def distance_log
		
	end

end
