def decode(string)
	return ActiveSupport::JSON.decode string
end

class PatientsController < ApplicationController

	# Gets the events for today
	def today
		str = ''
		Patient.first.medications.order('time asc').each do |med|
			med_days = decode med.days

			if not med_days.include? Time.now.wday - 1
				str += "passing  #{med.name}  --- #{med_days} XXX"
				next
			end

			now = Time.now
			min = med.time.hour * 60 + med.time.min
			min_now =  now.hour * 60 + now.min 

			if min < min_now
				str += "passing time #{med.name}  --- #{min} < #{min_now} XXX\n"
				next
			end

			render json: med
			return
		end

		render json: {status: 'failure'}
	end

end
