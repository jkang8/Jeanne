def decode(string)
	return ActiveSupport::JSON.decode string
end

class PatientsController < ApplicationController

	# Gets the events for today
	def today
		str = "#{Time.now.wday} <br/>"
		Patient.first.medications.order('time asc').each do |med|
			med_days = decode med.days

			if not med_days.include? Time.now.wday
				str += "passing  #{med.name}  --- #{med_days} <br/>"
				next
			end

			now = Time.now
			min = med.time.hour * 60 + med.time.min
			min_now =  now.hour * 60 + now.min 

			if min < min_now
				str += "passing time #{med.name}  --- #{min} < #{min_now} <br/>"
				next
			end

			render json: med
			return
		end

		if params[:debug]
			render text: str
			return
		end

		render json: {status: 'failure'}
		# render json: {
		# 			status: 'success',
		#  			time: (Time.now.to_i + 120),
		#  			name: 'Advil',
		#  			amount: 2
		#  		}
	end


	def delete_alert
		alert = Alert.find(params[:id])
		alert.delete
		render text: 'success'
	end


	def create_alert
		alert = Alert.new
		
		# alert.message = params[:message]
		alert.message = "#{Patient.first.name} needs your attention";
		alert.save

		message_user alert.message, params[:phone]

		render text: 'success'
	end

	def get_alerts


		id = params[:id]

		Alert.all.each do | alert | 
			if id.to_i and alert.id > id.to_i
				render alert
				return
			end
		end

		render text: 'none'
	end



	def message_user(message, phone)
		phone ||= '8587408886'
		params = {message: message, number: phone}
		Net::HTTP.post_form(URI.parse('http://textbelt.com/text'), params)
	end


end
