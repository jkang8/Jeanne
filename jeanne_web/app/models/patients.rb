class Patients < ActiveRecord::Base
	has_many :caregivers

	def distance_log

	end

end
