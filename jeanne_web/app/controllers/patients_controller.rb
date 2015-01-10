def decode(string)
	return ActiveSupport::JSON.decode string
end

class PatientsController < ApplicationController

	# Gets the events for today
	def today
		times = []
		Patient.first.medications.each do |med|
			med_days = decode med.days
			if not med_days.include? Time.now.wday
				next
			end
			
			med_times = decode med.times
			med_times.each do |time|
				times << {
					time: time.to_time.to_i,
					name: med.name,
					amount: med.amount
				}

			end
		end

		render json: times.first
	end

end
