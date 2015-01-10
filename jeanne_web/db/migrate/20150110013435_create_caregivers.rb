class CreateCaregivers < ActiveRecord::Migration
  def change
    create_table :caregivers do |t|
  	  t.string :name
  	  t.string :username
  	  t.string :email
  	  t.belongs_to :patient

      t.timestamps
    end
  end
end
