class CreateReminders < ActiveRecord::Migration
  def change
    create_table :reminders do |t|
      t.string :type
      t.string :description
   	  t.string :frequency

      t.belongs_to :patient
      t.time :time

      t.timestamps
    end
  end
end
