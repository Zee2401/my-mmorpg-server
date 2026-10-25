using Mirror;
using System;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Manages social features: friends, parties, guilds, chat
/// </summary>
public class SocialSystem : NetworkBehaviour
{
    public static SocialSystem Instance { get; private set; }
    
    [Header("Social Settings")]
    public int maxFriends = 100;
    public int maxPartySize = 5;
    public int maxGuildSize = 50;
    
    [SyncVar(hook = nameof(OnFriendsChanged))]
    public List<string> friends = new List<string>();
    
    [SyncVar(hook = nameof(OnPartyChanged))]
    public List<string> partyMembers = new List<string>();
    
    [SyncVar(hook = nameof(OnGuildChanged))]
    public GuildData guild;
    
    [SyncVar(hook = nameof(OnPendingRequestsChanged))]
    public List<string> pendingFriendRequests = new List<string>();
    
    [SyncVar(hook = nameof(OnBlockedPlayersChanged))]
    public List<string> blockedPlayers = new List<string>();
    
    private Dictionary<string, PlayerController> onlinePlayers = new Dictionary<string, PlayerController>();
    private PlayerController localPlayer;
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
    }
    
    private void Start()
    {
        // Find local player
        localPlayer = FindObjectOfType<PlayerController>();
        if (localPlayer != null && localPlayer.isLocalPlayer)
        {
            RegisterPlayer(localPlayer);
        }
    }
    
    public void RegisterPlayer(PlayerController player)
    {
        if (!onlinePlayers.ContainsKey(player.playerName))
        {
            onlinePlayers[player.playerName] = player;
        }
    }
    
    public void UnregisterPlayer(PlayerController player)
    {
        if (onlinePlayers.ContainsKey(player.playerName))
        {
            onlinePlayers.Remove(player.playerName);
        }
    }
    
    public List<PlayerController> GetOnlinePlayers()
    {
        return new List<PlayerController>(onlinePlayers.Values);
    }
    
    public PlayerController GetPlayer(string playerName)
    {
        onlinePlayers.TryGetValue(playerName, out PlayerController player);
        return player;
    }
    
    public bool IsOnline(string playerName)
    {
        return onlinePlayers.ContainsKey(playerName);
    }
    
    #region Friends
    
    public bool IsFriend(string playerName)
    {
        return friends.Contains(playerName);
    }
    
    public bool CanAddFriend(string playerName)
    {
        // Can't add self
        if (localPlayer != null && playerName == localPlayer.playerName) return false;
        
        // Already friends
        if (IsFriend(playerName)) return false;
        
        // Already requested
        if (pendingFriendRequests.Contains(playerName)) return false;
        
        // Blocked
        if (IsBlocked(playerName)) return false;
        
        // At max friends
        if (friends.Count >= maxFriends) return false;
        
        return true;
    }
    
    public void SendFriendRequest(string playerName)
    {
        if (!CanAddFriend(playerName)) return;
        
        if (isServer)
        {
            InternalSendFriendRequest(playerName);
        }
        else
        {
            CmdSendFriendRequest(playerName);
        }
    }
    
    [Command]
    private void CmdSendFriendRequest(string playerName)
    {
        InternalSendFriendRequest(playerName);
    }
    
    private void InternalSendFriendRequest(string playerName)
    {
        // Add to pending requests
        if (!pendingFriendRequests.Contains(playerName))
        {
            pendingFriendRequests.Add(playerName);
        }
        
        // Notify target player
        PlayerController target = GetPlayer(playerName);
        if (target != null && target.connectionToClient != null)
        {
            target.TargetSendFriendRequest(connectionToClient.identity.netId, localPlayer.playerName);
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Friend Request Sent", playerName, NotificationType.Info);
        }
    }
    
    [TargetRpc]
    public void TargetSendFriendRequest(NetworkConnection target, string requesterName)
    {
        // Show friend request notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Friend Request", $"{requesterName} wants to be your friend", NotificationType.Info);
        }
        
        // Add to pending requests
        if (!pendingFriendRequests.Contains(requesterName))
        {
            pendingFriendRequests.Add(requesterName);
        }
    }
    
    public void AcceptFriendRequest(string playerName)
    {
        if (!pendingFriendRequests.Contains(playerName)) return;
        
        if (isServer)
        {
            InternalAcceptFriendRequest(playerName);
        }
        else
        {
            CmdAcceptFriendRequest(playerName);
        }
    }
    
    [Command]
    private void CmdAcceptFriendRequest(string playerName)
    {
        InternalAcceptFriendRequest(playerName);
    }
    
    private void InternalAcceptFriendRequest(string playerName)
    {
        // Remove from pending
        pendingFriendRequests.Remove(playerName);
        
        // Add to friends
        if (!friends.Contains(playerName))
        {
            friends.Add(playerName);
        }
        
        // Notify both players
        PlayerController target = GetPlayer(playerName);
        if (target != null && target.connectionToClient != null)
        {
            target.TargetFriendRequestAccepted(connectionToClient.identity.netId, localPlayer.playerName);
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Friend Added", playerName, NotificationType.Success);
        }
    }
    
    [TargetRpc]
    public void TargetFriendRequestAccepted(NetworkConnection target, string accepterName)
    {
        // Add to friends
        if (!friends.Contains(accepterName))
        {
            friends.Add(accepterName);
        }
        
        // Remove from pending
        pendingFriendRequests.Remove(accepterName);
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Friend Added", accepterName, NotificationType.Success);
        }
    }
    
    public void DeclineFriendRequest(string playerName)
    {
        if (!pendingFriendRequests.Contains(playerName)) return;
        
        if (isServer)
        {
            InternalDeclineFriendRequest(playerName);
        }
        else
        {
            CmdDeclineFriendRequest(playerName);
        }
    }
    
    [Command]
    private void CmdDeclineFriendRequest(string playerName)
    {
        InternalDeclineFriendRequest(playerName);
    }
    
    private void InternalDeclineFriendRequest(string playerName)
    {
        // Remove from pending
        pendingFriendRequests.Remove(playerName);
        
        // Notify requester
        PlayerController target = GetPlayer(playerName);
        if (target != null && target.connectionToClient != null)
        {
            target.TargetFriendRequestDeclined(connectionToClient.identity.netId, localPlayer.playerName);
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Request Declined", playerName, NotificationType.Warning);
        }
    }
    
    [TargetRpc]
    public void TargetFriendRequestDeclined(NetworkConnection target, string declinerName)
    {
        // Remove from pending
        pendingFriendRequests.Remove(declinerName);
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Request Declined", declinerName, NotificationType.Warning);
        }
    }
    
    public void RemoveFriend(string playerName)
    {
        if (!IsFriend(playerName)) return;
        
        if (isServer)
        {
            InternalRemoveFriend(playerName);
        }
        else
        {
            CmdRemoveFriend(playerName);
        }
    }
    
    [Command]
    private void CmdRemoveFriend(string playerName)
    {
        InternalRemoveFriend(playerName);
    }
    
    private void InternalRemoveFriend(string playerName)
    {
        // Remove from friends
        friends.Remove(playerName);
        
        // Notify other player
        PlayerController target = GetPlayer(playerName);
        if (target != null && target.connectionToClient != null)
        {
            target.TargetFriendRemoved(connectionToClient.identity.netId, localPlayer.playerName);
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Friend Removed", playerName, NotificationType.Info);
        }
    }
    
    [TargetRpc]
    public void TargetFriendRemoved(NetworkConnection target, string removerName)
    {
        // Remove from friends
        friends.Remove(removerName);
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Friend Removed", removerName, NotificationType.Info);
        }
    }
    
    #endregion
    
    #region Party
    
    public bool IsInParty(string playerName)
    {
        return partyMembers.Contains(playerName);
    }
    
    public bool IsPartyLeader(string playerName)
    {
        return partyMembers.Count > 0 && partyMembers[0] == playerName;
    }
    
    public bool CanInviteToParty(string playerName)
    {
        // Can't invite self
        if (localPlayer != null && playerName == localPlayer.playerName) return false;
        
        // Already in party
        if (IsInParty(playerName)) return false;
        
        // At max party size
        if (partyMembers.Count >= maxPartySize) return false;
        
        // Not online
        if (!IsOnline(playerName)) return false;
        
        return true;
    }
    
    public void InviteToParty(string playerName)
    {
        if (!CanInviteToParty(playerName)) return;
        
        if (isServer)
        {
            InternalInviteToParty(playerName);
        }
        else
        {
            CmdInviteToParty(playerName);
        }
    }
    
    [Command]
    private void CmdInviteToParty(string playerName)
    {
        InternalInviteToParty(playerName);
    }
    
    private void InternalInviteToParty(string playerName)
    {
        PlayerController target = GetPlayer(playerName);
        if (target != null && target.connectionToClient != null)
        {
            target.TargetPartyInvite(connectionToClient.identity.netId, localPlayer.playerName);
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Party Invite Sent", playerName, NotificationType.Info);
        }
    }
    
    [TargetRpc]
    public void TargetPartyInvite(NetworkConnection target, string inviterName)
    {
        // Show party invite notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Party Invite", $"{inviterName} invites you to a party", NotificationType.Info);
        }
    }
    
    public void AcceptPartyInvite(string playerName)
    {
        if (isServer)
        {
            InternalAcceptPartyInvite(playerName);
        }
        else
        {
            CmdAcceptPartyInvite(playerName);
        }
    }
    
    [Command]
    private void CmdAcceptPartyInvite(string playerName)
    {
        InternalAcceptPartyInvite(playerName);
    }
    
    private void InternalAcceptPartyInvite(string playerName)
    {
        PlayerController inviter = GetPlayer(playerName);
        if (inviter == null) return;
        
        // Check if inviter is in a party
        if (inviter.IsInParty())
        {
            // Add to party
            if (!partyMembers.Contains(localPlayer.playerName))
            {
                partyMembers.Add(localPlayer.playerName);
            }
            
            // Notify inviter
            inviter.TargetPartyMemberAdded(connectionToClient.identity.netId, localPlayer.playerName);
        }
        else
        {
            // Create new party
            partyMembers.Clear();
            partyMembers.Add(playerName);
            partyMembers.Add(localPlayer.playerName);
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Party Joined", playerName, NotificationType.Success);
        }
    }
    
    [TargetRpc]
    public void TargetPartyMemberAdded(NetworkConnection target, string newMemberName)
    {
        // Add to party
        if (!partyMembers.Contains(newMemberName))
        {
            partyMembers.Add(newMemberName);
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Party Member Joined", newMemberName, NotificationType.Success);
        }
    }
    
    public void LeaveParty()
    {
        if (!IsInParty(localPlayer.playerName)) return;
        
        if (isServer)
        {
            InternalLeaveParty();
        }
        else
        {
            CmdLeaveParty();
        }
    }
    
    [Command]
    private void CmdLeaveParty()
    {
        InternalLeaveParty();
    }
    
    private void InternalLeaveParty()
    {
        // Remove from party
        partyMembers.Remove(localPlayer.playerName);
        
        // Notify other members
        foreach (var memberName in partyMembers)
        {
            PlayerController member = GetPlayer(memberName);
            if (member != null && member.connectionToClient != null)
            {
                member.TargetPartyMemberLeft(connectionToClient.identity.netId, localPlayer.playerName);
            }
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Party Left", "You left the party", NotificationType.Info);
        }
    }
    
    [TargetRpc]
    public void TargetPartyMemberLeft(NetworkConnection target, string leftMemberName)
    {
        // Remove from party
        partyMembers.Remove(leftMemberName);
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Party Member Left", leftMemberName, NotificationType.Info);
        }
    }
    
    public void KickFromParty(string playerName)
    {
        if (!IsPartyLeader(localPlayer.playerName)) return;
        if (!IsInParty(playerName)) return;
        
        if (isServer)
        {
            InternalKickFromParty(playerName);
        }
        else
        {
            CmdKickFromParty(playerName);
        }
    }
    
    [Command]
    private void CmdKickFromParty(string playerName)
    {
        InternalKickFromParty(playerName);
    }
    
    private void InternalKickFromParty(string playerName)
    {
        // Remove from party
        partyMembers.Remove(playerName);
        
        // Notify kicked player
        PlayerController target = GetPlayer(playerName);
        if (target != null && target.connectionToClient != null)
        {
            target.TargetPartyKicked(connectionToClient.identity.netId, localPlayer.playerName);
        }
        
        // Notify other members
        foreach (var memberName in partyMembers)
        {
            PlayerController member = GetPlayer(memberName);
            if (member != null && member.connectionToClient != null)
            {
                member.TargetPartyMemberRemoved(connectionToClient.identity.netId, playerName);
            }
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Player Kicked", playerName, NotificationType.Warning);
        }
    }
    
    [TargetRpc]
    public void TargetPartyKicked(NetworkConnection target, string kickerName)
    {
        // Clear party
        partyMembers.Clear();
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Party Kicked", kickerName, NotificationType.Warning);
        }
    }
    
    [TargetRpc]
    public void TargetPartyMemberRemoved(NetworkConnection target, string removedMemberName)
    {
        // Remove from party
        partyMembers.Remove(removedMemberName);
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Party Member Removed", removedMemberName, NotificationType.Info);
        }
    }
    
    #endregion
    
    #region Guild
    
    public bool IsInGuild()
    {
        return guild != null;
    }
    
    public bool IsGuildLeader()
    {
        return guild != null && guild.leaderId == localPlayer.playerName;
    }
    
    public bool CanCreateGuild()
    {
        return !IsInGuild();
    }
    
    public void CreateGuild(string guildName, string motto)
    {
        if (!CanCreateGuild()) return;
        
        if (isServer)
        {
            InternalCreateGuild(guildName, motto);
        }
        else
        {
            CmdCreateGuild(guildName, motto);
        }
    }
    
    [Command]
    private void CmdCreateGuild(string guildName, string motto)
    {
        InternalCreateGuild(guildName, motto);
    }
    
    private void InternalCreateGuild(string guildName, string motto)
    {
        guild = new GuildData
        {
            id = Guid.NewGuid().ToString(),
            name = guildName,
            motto = motto,
            leaderId = localPlayer.playerName,
            members = new List<GuildMember> { new GuildMember { playerId = localPlayer.playerName, role = GuildRole.Leader } },
            level = 1,
            experience = 0,
            createdDate = DateTime.Now
        };
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Guild Created", guildName, NotificationType.Success);
        }
    }
    
    public void InviteToGuild(string playerName)
    {
        if (!IsGuildLeader()) return;
        if (!CanAddFriend(playerName)) return;
        
        if (isServer)
        {
            InternalInviteToGuild(playerName);
        }
        else
        {
            CmdInviteToGuild(playerName);
        }
    }
    
    [Command]
    private void CmdInviteToGuild(string playerName)
    {
        InternalInviteToGuild(playerName);
    }
    
    private void InternalInviteToGuild(string playerName)
    {
        PlayerController target = GetPlayer(playerName);
        if (target != null && target.connectionToClient != null)
        {
            target.TargetGuildInvite(connectionToClient.identity.netId, localPlayer.playerName, guild.name);
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Guild Invite Sent", playerName, NotificationType.Info);
        }
    }
    
    [TargetRpc]
    public void TargetGuildInvite(NetworkConnection target, string inviterName, string guildName)
    {
        // Show guild invite notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Guild Invite", $"{inviterName} invites you to join {guildName}", NotificationType.Info);
        }
    }
    
    #endregion
    
    #region Block
    
    public bool IsBlocked(string playerName)
    {
        return blockedPlayers.Contains(playerName);
    }
    
    public void BlockPlayer(string playerName)
    {
        if (IsBlocked(playerName)) return;
        
        if (isServer)
        {
            InternalBlockPlayer(playerName);
        }
        else
        {
            CmdBlockPlayer(playerName);
        }
    }
    
    [Command]
    private void CmdBlockPlayer(string playerName)
    {
        InternalBlockPlayer(playerName);
    }
    
    private void InternalBlockPlayer(string playerName)
    {
        // Add to blocked
        if (!blockedPlayers.Contains(playerName))
        {
            blockedPlayers.Add(playerName);
        }
        
        // Remove from friends
        friends.Remove(playerName);
        
        // Remove from party
        partyMembers.Remove(playerName);
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Player Blocked", playerName, NotificationType.Warning);
        }
    }
    
    public void UnblockPlayer(string playerName)
    {
        if (!IsBlocked(playerName)) return;
        
        if (isServer)
        {
            InternalUnblockPlayer(playerName);
        }
        else
        {
            CmdUnblockPlayer(playerName);
        }
    }
    
    [Command]
    private void CmdUnblockPlayer(string playerName)
    {
        InternalUnblockPlayer(playerName);
    }
    
    private void InternalUnblockPlayer(string playerName)
    {
        // Remove from blocked
        blockedPlayers.Remove(playerName);
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Player Unblocked", playerName, NotificationType.Info);
        }
    }
    
    #endregion
    
    #region Chat
    
    public void SendChatMessage(string message, ChatChannel channel = ChatChannel.General)
    {
        if (string.IsNullOrEmpty(message)) return;
        
        if (isServer)
        {
            InternalSendChatMessage(message, channel);
        }
        else
        {
            CmdSendChatMessage(message, channel);
        }
    }
    
    [Command]
    private void CmdSendChatMessage(string message, ChatChannel channel)
    {
        InternalSendChatMessage(message, channel);
    }
    
    private void InternalSendChatMessage(string message, ChatChannel channel)
    {
        // Validate message
        if (string.IsNullOrEmpty(message)) return;
        
        // Create chat message
        ChatMessage chatMessage = new ChatMessage
        {
            senderId = localPlayer.netId.ToString(),
            senderName = localPlayer.playerName,
            message = message,
            channel = channel.ToString(),
            timestamp = DateTime.Now
        };
        
        // Send to appropriate channel
        switch (channel)
        {
            case ChatChannel.General:
                RpcReceiveChatMessage(chatMessage);
                break;
            case ChatChannel.Party:
                foreach (var memberName in partyMembers)
                {
                    PlayerController member = GetPlayer(memberName);
                    if (member != null && member.connectionToClient != null)
                    {
                        member.TargetReceiveChatMessage(connectionToClient.identity.netId, chatMessage);
                    }
                }
                break;
            case ChatChannel.Guild:
                if (guild != null)
                {
                    foreach (var member in guild.members)
                    {
                        PlayerController memberPlayer = GetPlayer(member.playerId);
                        if (memberPlayer != null && memberPlayer.connectionToClient != null)
                        {
                            memberPlayer.TargetReceiveChatMessage(connectionToClient.identity.netId, chatMessage);
                        }
                    }
                }
                break;
            case ChatChannel.Whisper:
                // Handle whisper separately
                break;
            case ChatChannel.Trade:
                // Handle trade chat separately
                break;
        }
    }
    
    [ClientRpc]
    private void RpcReceiveChatMessage(ChatMessage message)
    {
        // Display message in chat
        if (UIManager.Instance != null)
        {
            UIManager.Instance.AddChatMessage(message.senderName, message.message, message.channel);
        }
    }
    
    [TargetRpc]
    public void TargetReceiveChatMessage(NetworkConnection target, ChatMessage message)
    {
        // Display message in chat
        if (UIManager.Instance != null)
        {
            UIManager.Instance.AddChatMessage(message.senderName, message.message, message.channel);
        }
    }
    
    #endregion
    
    // SyncVar hooks
    private void OnFriendsChanged(List<string> oldFriends, List<string> newFriends)
    {
        friends = newFriends;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateSocialUI();
        }
    }
    
    private void OnPartyChanged(List<string> oldParty, List<string> newParty)
    {
        partyMembers = newParty;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdatePartyUI();
        }
    }
    
    private void OnGuildChanged(GuildData oldGuild, GuildData newGuild)
    {
        guild = newGuild;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateGuildUI();
        }
    }
    
    private void OnPendingRequestsChanged(List<string> oldRequests, List<string> newRequests)
    {
        pendingFriendRequests = newRequests;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateSocialUI();
        }
    }
    
    private void OnBlockedPlayersChanged(List<string> oldBlocked, List<string> newBlocked)
    {
        blockedPlayers = newBlocked;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateSocialUI();
        }
    }
}

[Serializable]
public class ChatMessage
{
    public string senderId;
    public string senderName;
    public string message;
    public string channel;
    public DateTime timestamp;
}

[Serializable]
public class GuildData
{
    public string id;
    public string name;
    public string motto;
    public string leaderId;
    public List<GuildMember> members;
    public int level;
    public long experience;
    public DateTime createdDate;
    public string icon;
    public string tag;
    public GuildRank rank;
}

[Serializable]
public class GuildMember
{
    public string playerId;
    public string playerName;
    public GuildRole role;
    public DateTime joinDate;
    public int contributionPoints;
    public int rankPoints;
}

public enum ChatChannel
{
    General,
    Party,
    Guild,
    Whisper,
    Trade,
    Local,
    System,
    Emote
}

public enum GuildRole
{
    Member,
    Officer,
    Leader
}

public enum GuildRank
{
    New,
    Rising,
    Established,
    Renowned,
    Legendary
}

public enum SocialStatus
{
    Online,
    Offline,
    Away,
    Busy,
    Invisible
}

public enum SocialRelationship
{
    Stranger,
    Friend,
    PartyMember,
    GuildMember,
    Blocked,
    Ignored
}

public enum SocialAction
{
    None,
    AddFriend,
    RemoveFriend,
    InviteToParty,
    KickFromParty,
    InviteToGuild,
    KickFromGuild,
    Block,
    Unblock,
    Whisper,
    Trade
}

public enum SocialNotification
{
    None,
    FriendRequest,
    PartyInvite,
    GuildInvite,
    TradeRequest,
    Message
}

public enum SocialPrivacy
{
    Public,
    FriendsOnly,
    GuildOnly,
    Private
}

public enum SocialVisibility
{
    Visible,
    Hidden,
    FriendsOnly,
    GuildOnly,
    Private
}

public enum SocialPermission
{
    Allow,
    Deny,
    Ask,
    Ignore
}

public enum SocialFilter
{
    All,
    Friends,
    Party,
    Guild,
    Nearby,
    Trade
}

public enum SocialSort
{
    Name,
    Level,
    Class,
    Status,
    LastOnline
}

public enum SocialTab
{
    Friends,
    Party,
    Guild,
    Nearby,
    Chat,
    Social
}
