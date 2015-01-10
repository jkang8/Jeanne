class ApplicationController < ActionController::Base
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
  protect_from_forgery with: :exception

  def login
  end

  def init
  	if Caregivers.all.count > 1
	  	Caregivers.first.destroy
		Patients.first.destroy
	end

  	caregiver = Caregivers.new
  	caregiver.username = 'Alex'
  	caregiver.password = '1'
  	caregiver.email= 'aprowe@ucsc.edu'

  	patient = Patients.new
  	patient.name = "Cara Jeanne"
  	patient.birthdate = '12/12/1930'.to_date
  	patient.save
  	caregiver.patient_id = patient.id

	logger.debug(caregiver.patient.id)

  	caregiver.save

  	redirect_to ''

  end

end
