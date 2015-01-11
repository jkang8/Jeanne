# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20150110052831) do

  create_table "caregivers", force: true do |t|
    t.string   "name"
    t.string   "username"
    t.string   "password"
    t.string   "email"
    t.integer  "patient_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "caregivers_patients", force: true do |t|
    t.integer  "caregiver_id"
    t.integer  "patient_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "locations", force: true do |t|
    t.integer  "patient_id"
    t.decimal  "latitude"
    t.decimal  "longitude"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "medications", force: true do |t|
    t.string   "name"
    t.string   "description"
    t.integer  "amount"
    t.integer  "patient_id"
    t.time     "time"
    t.string   "days"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "patients", force: true do |t|
    t.string   "name"
    t.string   "username"
    t.date     "birthdate"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "reminders", force: true do |t|
    t.string   "type"
    t.string   "description"
    t.string   "frequency"
    t.integer  "patient_id"
    t.time     "time"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

end
