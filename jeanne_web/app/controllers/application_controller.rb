class ApplicationController < ActionController::Base
    # Prevent CSRF attacks by raising an exception.
    # For APIs, you may want to use :null_session instead.
    protect_from_forgery with: :exception

    def login

        if params
            @username = params[:username]
            @password = params[:password]
            caregiver = Caregiver.find_by_username(@username)
            if caregiver and caregiver.password == @password
                session[:username] = caregiver.username
            end
        end

        if user
            redirect_to '/home'
        end
    end

    def logout
        session[:username] = nil
        redirect_to ''
    end


    def home
        @user = user

        if not @user
            redirect_to ''
        end

        @patient = user.patient
    end

    def clear
        session[:username] = nil
        redirect_to ''
    end


    def init
        if Caregiver.all.count > 1
            Caregiver.all.each do |giver|
                giver.destroy
            end

            Patient.all.each do |patients|
                patients.destroy
            end

            Medication.all.each do |patients|
                patients.destroy
            end

            Alert.all.each do |patients|
                patients.destroy
            end

        end

        patient = Patient.new
        patient.name = "Cara Jeanne"
        patient.birthdate = '12/12/1930'.to_date
        patient.save

        caregiver = Caregiver.new
        caregiver.username = 'Alex'
        caregiver.password = '1'
        caregiver.email= 'aprowe@ucsc.edu'
        caregiver.patient = patient
        caregiver.save


        medication = Medication.new 
        medication.name = 'Viagra'
        medication.description = 'Long White Pill'
        medication.time = Time.now
        medication.days = [0,1,2,3,4,5,6,7].to_json
        medication.patient = patient
        medication.amount = 2
        medication.save


        alert = Alert.new 
        alert.message = 'Your patient need your attention'
        alert.save


        redirect_to ''
    end

    def user
        return Caregiver.find_by_username session[:username]
    end

end
