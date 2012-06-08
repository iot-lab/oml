#!/usr/bin/ruby
#
# This little example triggers a bug in the OML server by sending an empy string 
# as the last parameter
#

require "#{ENV['TOPSRCDIR']}/lib/client/ruby/oml4r.rb"
require 'open3'

class Issue610MP < OML4R::MPBase
  name :test
  param :index, :type => :long
  param :label
end

def run
  # Find a free port, just in case a legit oml2-server already runs on the
  # machine

  puts "Test case for Issue #610"

  test_result = 0
  for port in 3003..3013 do
    begin 
      # Test the port
      server = TCPServer.new(port)
      # If we reach below without exception, we're fine
      confirmedPort = port
      server.close

    rescue Exception => ex
      puts "Port #{port}: #{ex}"

    end
    break if confirmedPort != nil
  end

  if confirmedPort == nil
    return 2

  else
    puts "Using port #{confirmedPort}"
    server = IO.popen(["../../server/oml2-server", "-D", ".", "-l", "#{confirmedPort}", "--logfile=-", :err=>[:child, :out]])
    sleep 1

    opts = { :domain => 'issue610', :id => "id", :app => 'app', :omlURL => "tcp:localhost:#{confirmedPort}" }
    OML4R::init(ARGV, opts)
    Issue610MP.inject 1, 'OK'
    Issue610MP.inject 2, ''
    OML4R::close

    sleep 2

    Process.kill("TERM", server.pid)
    server.readlines.each  { |line|
      if line =~ /Data item mismatch for table/
	puts "Issue #610 triggered: #{line}"
	test_result = 1
      end
    }
  end
  exit test_result
end

if __FILE__ == $PROGRAM_NAME then run; end

# vim: sw=2
