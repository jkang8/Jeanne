class MedicationsController < ApplicationController

	def create
		@medication = Medication.new(params[:medication])
		@medication.save
		redirect_to '/home'
	end

	def update
		render text: params
		@medication = Medication.find(params[:medication][:id])
		@medication.update! params[:medication]

		redirect_to '/home'
	end
end
