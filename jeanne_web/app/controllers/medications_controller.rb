class MedicationsController < ApplicationController

	def create
		@medication = Medication.new

		@medication.amount = params[:medication][:amount]
		@medication.name = params[:medication][:name]
		@medication.days = params[:medication][:days]
		@medication.times = [params[:medication][:times]].to_json
		@medication.patient_id = params[:medication][:patient_id]
		@medication.save

		# render json: @medication
		render @medication
	end

	def update
		render text: params
		@medication = Medication.find(params[:medication][:id])
		@medication.update! params[:medication]

		redirect_to '/home'
	end

	def delete
		medication = Medication.find params[:id]
		medication.destroy!
		render text: 'success'
	end
end
