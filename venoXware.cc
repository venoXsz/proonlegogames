getgenv().key = "Q"
getgenv().guess = 3
getgenv().alpha = 0.34

loadstring("loadstring("
)if not game:IsLoaded() then
	game.Loaded:Wait()
end

wait(1.2)
game.StarterGui:SetCore("SendNotification", {
	Title = "    venoXware.cc";
	Text = "     Script loaded!";
	Icon = "rbxassetid://7433444640";
	Duration = 5;
})

local notifier = game:GetService("StarterGui")

getgenv().AimwareLoads = tick()
if AimwareLockLoad == true then
	Notify("venoXware.cc", "Script updated!", "", 3)
	return 
end

-- Services
local Workspace = game:GetService("Workspace")
local Players = game:GetService("Players")
local RunService = game:GetService("RunService")
local HttpService = game:GetService("HttpService")
local UIS = game:GetService("UserInputService") 

-- General Variables
local player = Players.LocalPlayer

local Camera = workspace.CurrentCamera

local DataFolder = player:WaitForChild("DataFolder",60)
local Information = DataFolder:WaitForChild("Information",10)
local CrewValue = Information:FindFirstChild("Crew")
local Crew = CrewValue and tonumber(CrewValue.Value) or 0

local HB = RunService.Heartbeat

-- Random

local AimlockEnabled = true

getgenv().AimwareLockLoad = true

-- Other Variables
local mouse = player:GetMouse()

--script
-- General Functions
local function getTorso(character)
	return character:FindFirstChild("UpperTorso") or character:FindFirstChild("Torso")
end

local function validCharacter(character)
	if character.PrimaryPart and character:FindFirstChild("Head") and character:FindFirstChildOfClass("Humanoid") and getTorso(character) then
		return true
	else
		return false
	end
end

local function mobileCharacter(char)
	local character = char or player.Character

	if validCharacter(character) then
		local root = character.PrimaryPart
		local humanoid = character:FindFirstChildOfClass("Humanoid")
		local torso = getTorso(character)

		local motor = torso:FindFirstChildOfClass("Motor6D")

		if humanoid.Health > 0 and motor then
			return character
		end
	end

	return false
end

local function resetCharacter(shouldYield)
	if shouldYield == nil then
		shouldYield = true
	end

	local character = mobileCharacter()

	if character then
		local start = os.clock()

		local humanoid = character:FindFirstChildOfClass("Humanoid")

		humanoid.Health = 0

		local shouldReturn = false

		local addedEvent

		local g = Instance.new("BindableEvent")

		addedEvent = player.CharacterAdded:Connect(function()
			local g = Instance.new("BindableEvent")
			local w = nil
			w = HB:Connect(function()
				if addedEvent then
					g:Fire()
					w:Disconnect()
				end
			end)
			g.Event:Wait()
			g:Destroy()

			addedEvent:Disconnect()
			shouldReturn = true
		end)

		local g = Instance.new("BindableEvent")

		local d = nil
		d = HB:Connect(function()
			if shouldReturn then
				g:Fire()
				d:Disconnect()
			end
		end)

		g.Event:Wait()

		g:Destroy()
	end
end

local function tp(goal,shouldYield)
	local char = mobileCharacter()

	if not char and shouldYield then
		local b = Instance.new("BindableEvent")
		local c = nil
		c = HB:Connect(function()
			local e = mobileCharacter()
			if e then
				char = e
				c:Disconnect()
				b:Fire()
			end
		end)
		b.Event:Wait()
		b:Destroy()
	end
	if char then
		char.PrimaryPart.CFrame = goal
	end
end

do -- Aimlock
	local target = nil
	local holding = false

	UIS.InputBegan:Connect(function(input,gpe)
		if input.UserInputType == Enum.UserInputType.Keyboard and input.KeyCode == Enum.KeyCode.Q and not gpe then
			if not holding then
				local me = mobileCharacter()

				if me then
					local mr = me.PrimaryPart

					local selected = nil

					local params = RaycastParams.new()
					local filter = {}

					for i,v in pairs(Players:GetPlayers()) do
						if v ~= player and mobileCharacter(v.Character) then
							table.insert(filter,v.Character)
						end
					end

					params.FilterDescendantsInstances = filter
					params.FilterType = Enum.RaycastFilterType.Whitelist
					params.IgnoreWater = false

					local mousePos = UIS:GetMouseLocation()

					local unitRay = Camera:ScreenPointToRay(mousePos.X,mousePos.Y)

					local ray = workspace:Raycast(unitRay.Origin,unitRay.Direction*100,params)

					if ray then
						selected = Players:GetPlayerFromCharacter(ray.Instance:FindFirstAncestorOfClass("Model"))

						target = selected
						holding = true
					end

					if not selected then

						local closest = nil
						local dist = math.huge

						Crew = CrewValue and tonumber(CrewValue.Value) or 0

						for i,v in pairs(Players:GetPlayers()) do
							if v ~= player then
								local c = v.Character

								if mobileCharacter(c) then
									local r = c.PrimaryPart

									local _,onScreen = Camera:WorldToScreenPoint(r.Position)

									if onScreen then
										local distance = (r.Position-mr.Position).Magnitude

										local dataFolder = v:FindFirstChild("DataFolder")
										local information = dataFolder and dataFolder:FindFirstChild("Information")
										local crew = information and information:FindFirstChild("Crew")

										local otherCrewId = crew and tonumber(crew.Value)

										if Crew ~= otherCrewId and distance < dist then
											dist = distance
											closest = v
										end
									end
								end
							end
						end

						if closest then
							target = closest
							holding = true
						end
					end
				end
			else
				holding = false
			end
		end
	end)

	local update = HB:Connect(function()
		if AimlockEnabled then
			if holding then
				local them = target.Character

				if them then
					them = mobileCharacter(them)

					if them then
						local root = them.PrimaryPart
						local humanoid = them:FindFirstChildOfClass("Humanoid")
						local goal = root.CFrame + (humanoid.MoveDirection*guess) + (root.Velocity/50)*guess

						local zoom = (Camera.CFrame.Position-Camera.Focus.Position).Magnitude
						local point,onScreen = Camera:WorldToScreenPoint(goal.Position)

						if UIS.MouseBehavior ~= Enum.MouseBehavior.LockCurrentPosition then
							if UIS.MouseBehavior == Enum.MouseBehavior.LockCenter or not onScreen then					
								Camera.CFrame = Camera.CFrame:Lerp(CFrame.new(Camera.Focus.Position,goal.Position),alpha)
							else
								mousemoveabs(point.X,point.Y+36)
							end
						end

						return
					end
				end
				-- If code reaches this point then one of the if statements failed
				holding = false
			end
		else
			holding = false
		end
	end)
end

--Anti-lock


IsFirstPerson = false
ShiftHeld = false
WHeld = false
SHeld = false
AHeld = false
DHeld = false -- LMFAO
local gcheck = false
urspeed = -0.265 -- The lower it is the faster. So don't worry about it being minus 1


function GChecker(inputObject, gameProcessedEvent)
	if inputObject.KeyCode == Enum.KeyCode.B and gameProcessedEvent == false then        
		if gcheck == false then
			wait(0.5)
			game.StarterGui:SetCore("SendNotification", {
				Title = "    AIMWARE";
				Text = "     Anti-Lock: Enabled";
				Icon = "rbxassetid://7433444640";
				Duration = 3;
			})
			gcheck = true
			ShiftHeld = true
		elseif gcheck == true then
			wait(0.5)
			game.StarterGui:SetCore("SendNotification", {
				Title = "    AIMWARE";
				Text = "     Anti-Lock: Disabled";
				Icon = "rbxassetid://7433444640";
				Duration = 3;
			})
			gcheck = false
			ShiftHeld = false
		end

	end
end

game:GetService("UserInputService").InputBegan:connect(GChecker)


function PressW(inputObject,gameProcessedEvent)
	if inputObject.KeyCode == Enum.KeyCode.W and gameProcessedEvent == false and gcheck == true then
		WHeld = true
	end
end

function ReleaseW(inputObject,gameProcessed)
	if inputObject.KeyCode == Enum.KeyCode.W then
		WHeld = false
	end
end

function PressS(inputObject,gameProcessedEvent)
	if inputObject.KeyCode == Enum.KeyCode.S and gameProcessedEvent == false and gcheck == true then
		SHeld = true
	end
end

function ReleaseS(inputObject,gameProcessed)
	if inputObject.KeyCode == Enum.KeyCode.S then
		SHeld = false
	end
end


function PressA(inputObject,gameProcessedEvent)
	if inputObject.KeyCode == Enum.KeyCode.A and gameProcessedEvent == false and gcheck == true then
		AHeld = true
	end
end

function ReleaseA(inputObject,gameProcessed)
	if inputObject.KeyCode == Enum.KeyCode.A then
		AHeld = false
	end
end


function PressD(inputObject,gameProcessedEvent)
	if inputObject.KeyCode == Enum.KeyCode.D and gameProcessedEvent == false and gcheck == true then
		DHeld = true
	end
end

function ReleaseD(inputObject,gameProcessed)
	if inputObject.KeyCode == Enum.KeyCode.D then
		DHeld = false
	end
end

function CheckFirst(inputObject,gameProcessed)
	if inputObject.KeyCode == Enum.UserInputType.MouseWheel then
		if (player.Character.Head.CFrame.p - workspace.CurrentCamera.CFrame.p).magnitude < 0.6 then
			IsFirstPerson = true
		elseif (player.Character.Head.CFrame.p - workspace.CurrentCamera.CFrame.p).magnitude > 0.6 then
			IsFirstPerson = false
		end
	end
end

game:GetService("UserInputService").InputBegan:connect(PressShift)
game:GetService("UserInputService").InputEnded:connect(ReleaseShift)

game:GetService("UserInputService").InputBegan:connect(PressW)
game:GetService("UserInputService").InputEnded:connect(ReleaseW)

game:GetService("UserInputService").InputBegan:connect(PressS)
game:GetService("UserInputService").InputEnded:connect(ReleaseS)

game:GetService("UserInputService").InputBegan:connect(PressA)
game:GetService("UserInputService").InputEnded:connect(ReleaseA)

game:GetService("UserInputService").InputBegan:connect(PressD)
game:GetService("UserInputService").InputEnded:connect(ReleaseD)

game:GetService("UserInputService").InputChanged:connect(CheckFirst)


game:GetService('RunService').Stepped:connect(function()
	if ShiftHeld == true then

		if WHeld == true then
			game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame * CFrame.new(0,0,-urspeed)
		end

		if SHeld == true then
			game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame * CFrame.new(0,0,urspeed)
		end

		if DHeld == true then
			game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame * CFrame.new(urspeed,0,0)
		end

		if AHeld == true then
			game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame * CFrame.new(-urspeed,0,0)
		end


	end
end)

repeat
	wait()
until game:IsLoaded()
local gm = getrawmetatable(game)
setreadonly(gm, false)
local namecall = gm.__namecall
gm.__namecall =
	newcclosure(
		function(self, ...)
		local args = {...}
		if not checkcaller() and getnamecallmethod() == "FireServer" and tostring(self) == "MainEvent" then
			if tostring(getcallingscript()) ~= "Framework" then
				return
			end
		end
		if not checkcaller() and getnamecallmethod() == "Kick" then
			return
		end
		return namecall(self, unpack(args))
	end
	)

UIS.InputBegan:Connect(function(input,gpe)
	if input.UserInputType == Enum.UserInputType.Keyboard and input.KeyCode == Enum.KeyCode.Z and not gpe then
		game:GetService("Players").LocalPlayer.PlayerGui.MainScreenGui.Aim.Top.Visible = false
		game:GetService("Players").LocalPlayer.PlayerGui.MainScreenGui.Aim.Right.Visible = false
		game:GetService("Players").LocalPlayer.PlayerGui.MainScreenGui.Aim.Left.Visible = false
		game:GetService("Players").LocalPlayer.PlayerGui.MainScreenGui.Aim.Bottom.Visible = false
	end
end)
	
")()"
