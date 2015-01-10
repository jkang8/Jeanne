class Patients < ActiveRecord::Base
	has_many :caregivers

end
