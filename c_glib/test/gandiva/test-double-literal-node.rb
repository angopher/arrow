# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

class TestGandivaDoubleLiteralNode < Test::Unit::TestCase
  def setup
    omit("Gandiva is required") unless defined?(::Gandiva)
    @value = 1.5
    @node = Gandiva::DoubleLiteralNode.new(@value)
  end

  def test_value
    assert_equal(@value, @node.value)
  end

  def test_return_type
    assert_equal(Arrow::DoubleDataType.new, @node.return_type)
  end
end
